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

#include "gandiva/timestamp_ir.h"

#include <unordered_set>

namespace gandiva {

/*static*/ int64_t TimestampIR::UnitsPerSecond(arrow::TimeUnit::type unit) {
  switch (unit) {
    case arrow::TimeUnit::MILLI:
      return 1000;
    case arrow::TimeUnit::MICRO:
      return 1000000;
    case arrow::TimeUnit::NANO:
      return 1000000000;
    default:
      return 1;
  }
}

/*static*/ int64_t TimestampIR::UnitsPerMilli(arrow::TimeUnit::type unit) {
  switch (unit) {
    case arrow::TimeUnit::MILLI:
      return 1;
    case arrow::TimeUnit::MICRO:
      return 1000;
    case arrow::TimeUnit::NANO:
      return 1000000;
    default:
      return 1;
  }
}

// The complete set of function names that exist in the precompiled bitcode
// (precompiled/timestamp_unit_ops.cc), used to validate remapped names in
// LLVMGenerator::ResolveTimestampPcName().
static std::unordered_set<std::string> BuildAllFunctionNames() {
  std::unordered_set<std::string> names;
  const char* suffixes[] = {"_us", "_ns"};

  // Fixed-unit timestampadd (4 arg-order variants each)
  static const struct { const char* name; } kFixedAdds[] = {
      {"timestampaddSecond"}, {"timestampaddMinute"}, {"timestampaddHour"},
      {"timestampaddDay"},    {"timestampaddWeek"},
  };
  // Calendar-based timestampadd (4 arg-order variants each)
  static const char* kCalendarAdds[] = {
      "timestampaddMonth", "timestampaddQuarter", "timestampaddYear",
  };
  // Extract functions
  static const char* kExtracts[] = {
      "extractMillennium", "extractCentury", "extractDecade", "extractYear",
      "extractQuarter",    "extractMonth",   "extractWeek",   "extractDay",
      "extractHour",       "extractMinute",  "extractSecond", "extractDoy",
      "extractDow",        "extractEpoch",
  };
  // date_trunc functions
  static const char* kTruncs[] = {
      "date_trunc_Millennium", "date_trunc_Century", "date_trunc_Decade",
      "date_trunc_Year",       "date_trunc_Quarter", "date_trunc_Month",
      "date_trunc_Week",       "date_trunc_Day",     "date_trunc_Hour",
      "date_trunc_Minute",     "date_trunc_Second",
  };
  // timestampdiff functions
  static const char* kDiffs[] = {
      "timestampdiffSecond",  "timestampdiffMinute", "timestampdiffHour",
      "timestampdiffDay",     "timestampdiffWeek",   "timestampdiffMonth",
      "timestampdiffQuarter", "timestampdiffYear",
  };
  // Date arithmetic (all count_first=false except date_add/add which have both)
  static const struct {
    const char* name;
    bool count_first;
  } kDateArith[] = {
      {"date_add", true},  {"add", true},      {"date_add", false},
      {"add", false},      {"date_sub", false}, {"subtract", false},
      {"date_diff", false},
  };

  for (const auto* sfx : suffixes) {
    for (const auto& fa : kFixedAdds) {
      names.insert(std::string(fa.name) + "_int32_timestamp" + sfx);
      names.insert(std::string(fa.name) + "_int64_timestamp" + sfx);
      names.insert(std::string(fa.name) + "_timestamp_int32" + sfx);
      names.insert(std::string(fa.name) + "_timestamp_int64" + sfx);
    }
    for (const auto* ca : kCalendarAdds) {
      names.insert(std::string(ca) + "_int32_timestamp" + sfx);
      names.insert(std::string(ca) + "_timestamp_int32" + sfx);
      names.insert(std::string(ca) + "_int64_timestamp" + sfx);
      names.insert(std::string(ca) + "_timestamp_int64" + sfx);
    }
    for (const auto* ex : kExtracts) {
      names.insert(std::string(ex) + "_timestamp" + sfx);
    }
    for (const auto* tr : kTruncs) {
      names.insert(std::string(tr) + "_timestamp" + sfx);
    }
    for (const auto* di : kDiffs) {
      names.insert(std::string(di) + "_timestamp_timestamp" + sfx);
    }
    names.insert(std::string("months_between_timestamp_timestamp") + sfx);
    names.insert(std::string("datediff_timestamp_timestamp") + sfx);
    names.insert(std::string("castDATE_timestamp") + sfx);
    names.insert(std::string("castTIME_timestamp") + sfx);
    names.insert(std::string("last_day_from_timestamp") + sfx);
    for (const auto& da : kDateArith) {
      if (da.count_first) {
        names.insert(std::string(da.name) + "_int32_timestamp" + sfx);
        names.insert(std::string(da.name) + "_int64_timestamp" + sfx);
      } else {
        names.insert(std::string(da.name) + "_timestamp_int32" + sfx);
        names.insert(std::string(da.name) + "_timestamp_int64" + sfx);
      }
    }
    names.insert(std::string("to_utc_timezone_timestamp") + sfx);
    names.insert(std::string("from_utc_timezone_timestamp") + sfx);
    names.insert(std::string("castVARCHAR_timestamp_int64") + sfx);
    names.insert(std::string("next_day_from_timestamp") + sfx);
  }
  return names;
}

static const std::unordered_set<std::string>& AllFunctionNames() {
  static const std::unordered_set<std::string> names = BuildAllFunctionNames();
  return names;
}

/*static*/ bool TimestampIR::IsTimestampIRFunction(const std::string& name) {
  return AllFunctionNames().count(name) != 0;
}

}  // namespace gandiva
