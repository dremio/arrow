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
#include <unordered_set>

#include "arrow/type.h"
#include "gandiva/function_ir_builder.h"

namespace gandiva {

/// @brief Timestamp IR functions for unit-aware timestamp operations.
///
/// Follows the DecimalIR pattern: builds LLVM IR functions that handle
/// timestamp values in their native TimeUnit (ms, us, ns) without lossy
/// conversion.
///
/// Patterns:
/// - Pure IR: arithmetic generated entirely in IR (e.g., timestampaddSecond).
/// - Calendar wrapper: split into millis + remainder, call precompiled, recombine
///   (e.g., timestampaddMonth).
/// - Extract wrapper: convert to millis, call precompiled, return scalar
///   (e.g., extractMonth).
/// - Trunc wrapper: convert to millis, call precompiled truncation, scale back,
///   zero sub-milli remainder (e.g., date_trunc_Day).
/// - Diff wrapper: convert both inputs to millis, call precompiled, return scalar
///   (e.g., timestampdiffDay, months_between).
/// - Cast wrapper: convert to millis, call precompiled cast
///   (e.g., castDATE, castVARCHAR).
class TimestampIR : public FunctionIRBuilder {
 public:
  explicit TimestampIR(Engine* engine) : FunctionIRBuilder(engine) {}

  static Status AddFunctions(Engine* engine);
  static bool IsTimestampIRFunction(const std::string& function_name);
  static int64_t UnitsPerSecond(arrow::TimeUnit::type unit);
  static int64_t UnitsPerMilli(arrow::TimeUnit::type unit);

 private:
  // ts + count * fixed_constant (pure IR)
  Status BuildTimestampaddFixed(const std::string& fn, int64_t seconds_per_unit,
                                arrow::TimeUnit::type unit);

  // date_add/add/date_sub/subtract/date_diff and int64 timestampadd (pure IR)
  // seconds_per_count: positive for add, negative for sub
  Status BuildDateArithFixed(const std::string& fn, bool count_first,
                             int64_t seconds_per_count, arrow::TimeUnit::type unit,
                             llvm::Type* count_type);

  // ts + count * months via precompiled calendar math (split/recombine)
  Status BuildTimestampaddCalendar(const std::string& fn,
                                   const std::string& precompiled_fn,
                                   arrow::TimeUnit::type unit);

  // Generic calendar wrapper: handles both arg orders and int32/int64 count
  Status BuildTimestampaddCalendarGeneric(const std::string& fn,
                                          const std::string& precompiled_fn,
                                          arrow::TimeUnit::type unit,
                                          bool count_first,
                                          llvm::Type* count_type);

  // Extract: convert ts to millis, call precompiled, return int64
  // fn(int64 ts) -> int64
  Status BuildExtractWrapper(const std::string& fn,
                             const std::string& precompiled_fn,
                             arrow::TimeUnit::type unit);

  // date_trunc: convert ts to millis, call precompiled trunc, scale back
  // The truncation zeroes sub-unit data, so no remainder recombination.
  // fn(int64 ts) -> int64
  Status BuildTruncWrapper(const std::string& fn,
                           const std::string& precompiled_fn,
                           arrow::TimeUnit::type unit);

  // Diff: convert both ts inputs to millis, call precompiled, return scalar
  // fn(int64 ts1, int64 ts2) -> int32 or float64
  Status BuildDiffWrapper(const std::string& fn,
                          const std::string& precompiled_fn,
                          arrow::TimeUnit::type unit,
                          llvm::Type* return_type);

  // Cast: convert ts to millis, call precompiled cast (variable signatures)
  Status BuildCastFromTimestampWrapper(const std::string& fn,
                                       const std::string& precompiled_fn,
                                       arrow::TimeUnit::type unit,
                                       llvm::Type* return_type);

  // Timezone: split-recombine wrapper for to_utc/from_utc
  // fn(context, ts, tz_str, tz_len) -> ts
  Status BuildTimezoneWrapper(const std::string& fn,
                              const std::string& precompiled_fn,
                              arrow::TimeUnit::type unit);

  // castVARCHAR: scale ts to millis before formatting
  // fn(context, ts, len, &out_len) -> const char*
  Status BuildCastVARCHARWrapper(const std::string& fn,
                                 const std::string& precompiled_fn,
                                 arrow::TimeUnit::type unit);

  // next_day: scale ts to millis, call precompiled, return date64
  // fn(context, ts, day_str, day_len) -> int64
  Status BuildNextDayWrapper(const std::string& fn,
                             const std::string& precompiled_fn,
                             arrow::TimeUnit::type unit);

  // Floor division: ts / divisor rounded toward negative infinity.
  // C/LLVM SDiv truncates toward zero, which gives wrong millis for negative
  // timestamps with non-zero sub-ms components (e.g., SDiv(-456, 1000) = 0
  // instead of -1). This helper corrects the quotient.
  llvm::Value* FloorDiv(llvm::Value* ts, llvm::Value* divisor);

  // Floor division with remainder: returns {quotient, remainder} where
  // quotient * divisor + remainder == ts and 0 <= remainder < divisor.
  // Used by split-recombine wrappers (timezone, calendar add).
  std::pair<llvm::Value*, llvm::Value*> FloorDivRem(llvm::Value* ts,
                                                     llvm::Value* divisor);
};

}  // namespace gandiva
