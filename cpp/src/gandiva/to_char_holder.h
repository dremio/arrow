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

#pragma once

#include <memory>
#include <string>

#include "arrow/result.h"

#include "gandiva/execution_context.h"
#include "gandiva/function_holder.h"
#include "gandiva/node.h"
#include "gandiva/visibility.h"

namespace gandiva {

/// Function Holder for SQL 'to_char(timestamp, format)'
class GANDIVA_EXPORT ToCharTimestampHolder : public FunctionHolder {
 public:
  ~ToCharTimestampHolder() override = default;

  static Result<std::shared_ptr<ToCharTimestampHolder>> Make(const FunctionNode& node);

  static Result<std::shared_ptr<ToCharTimestampHolder>> Make(
      const std::string& sql_pattern);

  const char* operator()(ExecutionContext* context, int64_t timestamp_millis,
                         bool in_valid, bool* out_valid, int32_t* out_len);

 private:
  explicit ToCharTimestampHolder(const std::string& pattern) : pattern_(pattern) {}

  std::string pattern_;  // internal format string
};

}  // namespace gandiva
