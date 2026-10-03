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

#include "gandiva/time_ir.h"

#include <unordered_set>

namespace gandiva {

/*static*/ std::string TimeIR::UnitSuffix(arrow::TimeUnit::type unit) {
  return unit == arrow::TimeUnit::NANO ? "_ns" : "";
}

// Unit-specific function names that exist in the precompiled bitcode
// (precompiled/time_unit_ops.cc).
static const std::unordered_set<std::string>& UnitSpecificFunctionNames() {
  static const std::unordered_set<std::string> names = {
      "extractHour_time64_ns",
      "extractMinute_time64_ns",
      "extractSecond_time64_ns",
  };
  return names;
}

// time64 functions whose result does not depend on the time unit, as long as all
// time64 arguments share a unit (precompiled/arithmetic_ops.cc).
static const std::unordered_set<std::string>& UnitAgnosticFunctionNames() {
  static const std::unordered_set<std::string> names = {
      "equal_time64_time64",
      "not_equal_time64_time64",
      "less_than_time64_time64",
      "less_than_or_equal_to_time64_time64",
      "greater_than_time64_time64",
      "greater_than_or_equal_to_time64_time64",
      "isnull_time64",
      "isnotnull_time64",
      "is_distinct_from_time64_time64",
      "is_not_distinct_from_time64_time64",
  };
  return names;
}

/*static*/ bool TimeIR::IsTimeIRFunction(const std::string& name) {
  return UnitSpecificFunctionNames().count(name) != 0;
}

/*static*/ bool TimeIR::IsUnitAgnosticFunction(const std::string& pc_name) {
  return UnitAgnosticFunctionNames().count(pc_name) != 0;
}

}  // namespace gandiva
