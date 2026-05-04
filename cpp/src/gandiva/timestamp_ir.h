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

#include <string>

#include "arrow/type.h"

namespace gandiva {

/// @brief Registry of timestamp functions that have precompiled _us / _ns variants.
///
/// The _us and _ns wrapper functions are now generated at build time as precompiled
/// bitcode (precompiled/timestamp_unit_ops.cc) rather than dynamically via the LLVM
/// IR builder at JIT initialization time.
///
/// IsTimestampIRFunction() is still used by LLVMGenerator::ResolveTimestampPcName()
/// to validate that a remapped function name exists before returning it.
class TimestampIR {
 public:
  static bool IsTimestampIRFunction(const std::string& function_name);
  static int64_t UnitsPerSecond(arrow::TimeUnit::type unit);
  static int64_t UnitsPerMilli(arrow::TimeUnit::type unit);
};

}  // namespace gandiva
