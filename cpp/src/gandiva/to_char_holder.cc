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

#include "gandiva/to_char_holder.h"

#include <chrono>
#include <cstring>
#include <sstream>
#include <string>

#include "arrow/vendored/datetime.h"

#include "gandiva/date_utils.h"
#include "gandiva/node.h"

namespace gandiva {

Result<std::shared_ptr<ToCharTimestampHolder>> ToCharTimestampHolder::Make(
    const FunctionNode& node) {
  if (node.children().size() != 2) {
    return Status::Invalid("'to_char' function requires two parameters");
  }

  auto literal_pattern = dynamic_cast<LiteralNode*>(node.children().at(1).get());
  if (literal_pattern == nullptr) {
    return Status::Invalid(
        "'to_char' function requires a literal as the second parameter");
  }

  auto literal_type = literal_pattern->return_type()->id();
  if (literal_type != arrow::Type::STRING && literal_type != arrow::Type::BINARY) {
    return Status::Invalid(
        "'to_char' function requires a string literal as the second parameter");
  }

  auto pattern = std::get<std::string>(literal_pattern->holder());
  return Make(pattern);
}

Result<std::shared_ptr<ToCharTimestampHolder>> ToCharTimestampHolder::Make(
    const std::string& sql_pattern) {
  std::shared_ptr<std::string> transformed_pattern;
  ARROW_RETURN_NOT_OK(DateUtils::ToInternalFormat(sql_pattern, &transformed_pattern));
  return std::shared_ptr<ToCharTimestampHolder>(
      new ToCharTimestampHolder(*transformed_pattern));
}

const char* ToCharTimestampHolder::operator()(ExecutionContext* context,
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
  // in output unless explicitly requested via millisecond format specifiers
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
    std::string err_msg = std::string("Error formatting timestamp for given format: ") +
                          ex.what();
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
