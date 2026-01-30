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

#include "gandiva/parse_timestamp_holder.h"

#include <string>

#include "arrow/util/value_parsing.h"
#include "gandiva/date_utils.h"
#include "gandiva/execution_context.h"
#include "gandiva/node.h"

namespace gandiva {

Result<std::shared_ptr<ParseTimestampHolder>> ParseTimestampHolder::Make(
    const FunctionNode& node) {
  if (node.children().size() != 2) {
    return Status::Invalid("'parse_timestamp' function requires two parameters");
  }

  auto literal_pattern = dynamic_cast<LiteralNode*>(node.children().at(1).get());
  if (literal_pattern == nullptr) {
    return Status::Invalid(
        "'parse_timestamp' function requires a literal as the second parameter");
  }

  auto literal_type = literal_pattern->return_type()->id();
  if (literal_type != arrow::Type::STRING && literal_type != arrow::Type::BINARY) {
    return Status::Invalid(
        "'parse_timestamp' function requires a string literal as the second parameter");
  }

  auto pattern = std::get<std::string>(literal_pattern->holder());
  return Make(pattern);
}

Result<std::shared_ptr<ParseTimestampHolder>> ParseTimestampHolder::Make(
    const std::string& sql_pattern) {
  std::shared_ptr<std::string> transformed_pattern;
  ARROW_RETURN_NOT_OK(DateUtils::ToInternalFormat(sql_pattern, &transformed_pattern));
  return std::shared_ptr<ParseTimestampHolder>(
      new ParseTimestampHolder(*transformed_pattern));
}

int64_t ParseTimestampHolder::operator()(ExecutionContext* context, const char* data,
                                        int data_len, bool in_valid,
                                        bool* out_valid) {
  *out_valid = false;
  if (!in_valid) {
    return 0;
  }

  int64_t millis_since_epoch = 0;
  if (!::arrow::internal::ParseTimestampStrptime(
          data, data_len, pattern_.c_str(),
          /*ignore_time_in_day=*/false,
          /*allow_trailing_chars=*/true, ::arrow::TimeUnit::MILLI,
          &millis_since_epoch)) {
    return_error(context, data, data_len);
    return 0;
  }

  *out_valid = true;
  return millis_since_epoch;
}

void ParseTimestampHolder::return_error(ExecutionContext* context, const char* data,
                                       int data_len) {
  std::string err_msg =
      "Error parsing value " + std::string(data, data_len) + " for given format.";
  context->set_error_msg(err_msg.c_str());
}

}  // namespace gandiva
