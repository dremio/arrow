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

/// @brief Registry of time64 functions and their unit-specific precompiled variants.
///
/// Gandiva registers time64 signatures with time64[us]; the unsuffixed precompiled
/// function takes microseconds-of-day. Function signature matching ignores the time64
/// unit, so time64[ns] arguments match the same signature, and
/// LLVMGenerator::ResolveTimePcName() remaps the call to the _ns variant in
/// precompiled/time_unit_ops.cc.
///
/// Unlike timestamp, a time64[ns] call is never allowed to silently fall back to the
/// microsecond function: every time64 function must either have an _ns variant or be
/// unit-agnostic (e.g. comparisons, which only need both args to share a unit).
class TimeIR {
 public:
  /// Returns true if \p function_name is a unit-specific time64 variant
  /// (e.g. "extractHour_time64_ns").
  static bool IsTimeIRFunction(const std::string& function_name);

  /// Returns true if the time64 precompiled function \p pc_name produces the same
  /// result for any (single) time unit, so no unit-specific variant is needed.
  static bool IsUnitAgnosticFunction(const std::string& pc_name);

  /// Suffix of the unit-specific variant for \p unit; empty for the registered unit
  /// (MICRO).
  static std::string UnitSuffix(arrow::TimeUnit::type unit);
};

}  // namespace gandiva
