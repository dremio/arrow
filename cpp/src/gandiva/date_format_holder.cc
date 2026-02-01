// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "gandiva/date_format_holder.h"

#include <chrono>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>
#include <unordered_map>

#include "arrow/vendored/datetime.h"

#include "gandiva/node.h"

namespace gandiva {
namespace {

Status ToInternalJavaDateFormat(const std::string& format,
                                std::shared_ptr<std::string>* internal_format) {
  // A small subset of Java/Spark patterns used by Dremio's date_format.
  //
  // Note means/behavior:
  // - We intentionally preserve case. (MM=month, mm=minute)
  // - Quoted literals are supported with either single quotes (Java) or
  //   double quotes (existing Gandiva DateUtils conventions).
  static const std::unordered_map<std::string, std::string> kTokenMap = {
      // Year
      {"yyyy", "%Y"},
      {"yyy", "%Y"},
      {"yy", "%y"},
      {"y", "%Y"},

      // Month
      {"MMMM", "%B"},
      {"MMM", "%b"},
      {"MM", "%m"},
      {"M", "%m"},

      // Day
      {"dd", "%d"},
      {"d", "%d"},

      // Hour
      {"HH", "%H"},
      {"H", "%H"},
      {"hh", "%I"},
      {"h", "%I"},

      // Minute / Second
      {"mm", "%M"},
      {"m", "%M"},
      {"ss", "%S"},
      {"s", "%S"},

      // AM/PM
      {"a", "%p"},

      // Day name
      {"EEEE", "%A"},
      {"EEE", "%a"},

      // Milliseconds (date library extension; matches existing Gandiva mapping)
      {"SSS", "SSS"},
      {"SS", "SS"},
      {"S", "S"},

      // Timezone (best-effort; may depend on timestamp type semantics)
      {"Z", "%z"},
  };

  auto is_token_char = [](char c) { return std::isalpha(static_cast<unsigned char>(c)); };

  std::stringstream builder;
  bool in_quoted_text = false;
  char quote_char = '\0';

  for (size_t i = 0; i < format.size();) {
    char current = format[i];

    if ((current == '\'' || current == '"')) {
      if (!in_quoted_text) {
        in_quoted_text = true;
        quote_char = current;
        ++i;
        continue;
      }

      if (current == quote_char) {
        // For single-quote literals, treat doubled quotes ('') as a literal quote.
        if (quote_char == '\'' && (i + 1) < format.size() && format[i + 1] == '\'') {
          builder << '\'';
          i += 2;
          continue;
        }
        in_quoted_text = false;
        quote_char = '\0';
        ++i;
        continue;
      }
    }

    if (in_quoted_text) {
      builder << current;
      ++i;
      continue;
    }

    // Pass-through common separators.
    const std::string special_chars = "*-/,.;: _";
    if (special_chars.find_first_of(current) != std::string::npos) {
      builder << current;
      ++i;
      continue;
    }

    if (!is_token_char(current)) {
      return Status::Invalid("Invalid date format string '", format,
                             "' (unexpected character '", current, "')");
    }

    // Greedy match from current position.
    bool matched = false;
    size_t max_token_len = std::min<size_t>(5, format.size() - i);
    for (size_t len = max_token_len; len >= 1; --len) {
      const std::string token = format.substr(i, len);
      auto it = kTokenMap.find(token);
      if (it != kTokenMap.end()) {
        builder << it->second;
        i += len;
        matched = true;
        break;
      }
      if (len == 1) {
        break;
      }
    }

    if (!matched) {
      return Status::Invalid("Invalid date format string '", format,
                             "' at position ", static_cast<int64_t>(i));
    }
  }

  if (in_quoted_text) {
    return Status::Invalid("Invalid date format string '", format,
                           "' (unterminated quoted literal)");
  }

  internal_format->reset(new std::string(builder.str()));
  return Status::OK();
}

}  // namespace

Result<std::shared_ptr<DateFormatTimestampHolder>> DateFormatTimestampHolder::Make(
    const FunctionNode& node) {
  if (node.children().size() != 2) {
    return Status::Invalid("'date_format' function requires two parameters");
  }

  auto literal_pattern = dynamic_cast<LiteralNode*>(node.children().at(1).get());
  if (literal_pattern == nullptr) {
    return Status::Invalid(
        "'date_format' function requires a literal as the second parameter");
  }

  auto literal_type = literal_pattern->return_type()->id();
  if (literal_type != arrow::Type::STRING && literal_type != arrow::Type::BINARY) {
    return Status::Invalid(
        "'date_format' function requires a string literal as the second parameter");
  }

  auto pattern = std::get<std::string>(literal_pattern->holder());
  return Make(pattern);
}

Result<std::shared_ptr<DateFormatTimestampHolder>> DateFormatTimestampHolder::Make(
    const std::string& format_pattern) {
  std::shared_ptr<std::string> transformed_pattern;
  ARROW_RETURN_NOT_OK(ToInternalJavaDateFormat(format_pattern, &transformed_pattern));
  return std::shared_ptr<DateFormatTimestampHolder>(
      new DateFormatTimestampHolder(*transformed_pattern));
}

const char* DateFormatTimestampHolder::operator()(ExecutionContext* context,
                                                  int64_t timestamp_millis,
                                                  bool in_valid, bool* out_valid,
                                                  int32_t* out_len) {
  *out_valid = false;
  *out_len = 0;
  if (!in_valid) {
    return "";
  }

  using std::chrono::milliseconds;
  using std::chrono::seconds;

  // Convert to time point - use seconds precision to avoid fractional seconds
  // in output unless explicitly requested via millisecond format specifiers.
  const auto tp_millis = arrow_vendored::date::sys_time<milliseconds>{
      milliseconds{timestamp_millis}};
  const auto tp = arrow_vendored::date::floor<seconds>(tp_millis);

  std::ostringstream bufstream;
  bufstream.exceptions(std::ios::failbit | std::ios::badbit);

  std::string formatted;
  try {
    arrow_vendored::date::to_stream(bufstream, pattern_.c_str(), tp);
    formatted = bufstream.str();
  } catch (const std::exception& ex) {
    std::string err_msg =
        std::string("Error formatting timestamp for given format: ") + ex.what();
    context->set_error_msg(err_msg.c_str());
    return "";
  }

  auto* out = reinterpret_cast<char*>(context->arena()->Allocate(formatted.size()));
  if (out == nullptr) {
    context->set_error_msg("Could not allocate memory for output string");
    return "";
  }

  if (!formatted.empty()) {
    memcpy(out, formatted.data(), formatted.size());
  }
  *out_len = static_cast<int32_t>(formatted.size());
  *out_valid = true;
  return out;
}

}  // namespace gandiva
