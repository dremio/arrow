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

#include "arrow/status.h"
#include "arrow/util/logging.h"
#include "gandiva/engine.h"

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

// Unit suffix appended to precompiled function names.
static const char* UnitSuffix(arrow::TimeUnit::type unit) {
  switch (unit) {
    case arrow::TimeUnit::MICRO:
      return "_us";
    case arrow::TimeUnit::NANO:
      return "_ns";
    default:
      return "";
  }
}

// Fixed-unit timestampadd: ts + count * constant (pure IR)
struct FixedAdd {
  const char* name;
  int64_t seconds;
};
static const FixedAdd kFixedAdds[] = {
    {"timestampaddSecond", 1},  {"timestampaddMinute", 60},   {"timestampaddHour", 3600},
    {"timestampaddDay", 86400}, {"timestampaddWeek", 604800},
};

// Calendar-based timestampadd: split/recombine around precompiled millis fn
static const char* kCalendarAdds[] = {
    "timestampaddMonth",
    "timestampaddQuarter",
    "timestampaddYear",
};

// Extract functions: convert ts to millis, call precompiled, return int64
// pc_name pattern: {name}_timestamp
static const char* kExtracts[] = {
    "extractMillennium", "extractCentury", "extractDecade", "extractYear",
    "extractQuarter",    "extractMonth",   "extractWeek",   "extractDay",
    "extractHour",       "extractMinute",  "extractSecond", "extractDoy",
    "extractDow",        "extractEpoch",
};

// date_trunc functions: convert ts to millis, truncate, scale back (zero remainder)
// pc_name pattern: date_trunc_{Level}_timestamp
static const char* kTruncs[] = {
    "date_trunc_Millennium", "date_trunc_Century", "date_trunc_Decade", "date_trunc_Year",
    "date_trunc_Quarter",    "date_trunc_Month",   "date_trunc_Week",   "date_trunc_Day",
    "date_trunc_Hour",       "date_trunc_Minute",  "date_trunc_Second",
};

// timestampdiff: convert both inputs to millis, return int32
// pc_name pattern: {name}_timestamp_timestamp
static const char* kDiffs[] = {
    "timestampdiffSecond",  "timestampdiffMinute", "timestampdiffHour",
    "timestampdiffDay",     "timestampdiffWeek",   "timestampdiffMonth",
    "timestampdiffQuarter", "timestampdiffYear",
};

// Two-timestamp functions returning scalar
// months_between(ts,ts)->float64, datediff(ts,ts)->int32
struct TwoTsScalar {
  const char* name;
  bool returns_float;  // true=float64, false=int32
};
static const TwoTsScalar kTwoTsScalars[] = {
    {"months_between", true},
    {"datediff", false},
};

// Cast functions from timestamp
struct CastFromTs {
  const char* name;
  bool returns_i32;  // true=int32 (castTIME), false=int64 (castDATE)
};
static const CastFromTs kCastsFromTs[] = {
    {"castDATE", false},
    {"castTIME", true},
    {"last_day_from", false},  // last_day_from_timestamp(ts) -> date64
};

// date_add/add/date_sub/subtract/date_diff with timestamp:
// These are fixed-unit (1 day) arithmetic with varying arg orders and signs.
struct DateArith {
  const char* name;
  bool count_first;  // true=(int,ts), false=(ts,int)
  int64_t sign;      // +1 for add, -1 for sub
};
static const DateArith kDateArithEntries[] = {
    {"date_add", true, 1},    {"add", true, 1},        {"date_add", false, 1},
    {"add", false, 1},        {"date_sub", false, -1}, {"subtract", false, -1},
    {"date_diff", false, -1},
};

// Units to generate functions for.
static const arrow::TimeUnit::type kUnits[] = {
    arrow::TimeUnit::MICRO,
    arrow::TimeUnit::NANO,
};

// Build the deterministic set of all IR function names that AddFunctions will create.
// These names depend only on the static tables above, not on any engine state.
static std::unordered_set<std::string> BuildAllFunctionNames() {
  std::unordered_set<std::string> names;
  const char* suffixes[] = {"_us", "_ns"};
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
      names.insert(std::string(ca) + "_timestamp_int64" + sfx);
      names.insert(std::string(ca) + "_int64_timestamp" + sfx);
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
    for (const auto& ts2 : kTwoTsScalars) {
      names.insert(std::string(ts2.name) + "_timestamp_timestamp" + sfx);
    }
    for (const auto& c : kCastsFromTs) {
      names.insert(std::string(c.name) + "_timestamp" + sfx);
    }
    for (const auto& da : kDateArithEntries) {
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

// Thread-safe const set: initialized once via C++11 static local guarantee.
static const std::unordered_set<std::string>& AllFunctionNames() {
  static const std::unordered_set<std::string> names = BuildAllFunctionNames();
  return names;
}

/*static*/ bool TimestampIR::IsTimestampIRFunction(const std::string& name) {
  return AllFunctionNames().count(name) != 0;
}

Status TimestampIR::BuildTimestampaddFixed(const std::string& function_name,
                                           int64_t seconds_per_unit,
                                           arrow::TimeUnit::type time_unit) {
  auto i32 = types()->i32_type();
  auto i64 = types()->i64_type();
  auto function = BuildFunction(function_name, i64, {{"count", i32}, {"ts", i64}});

  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  auto count = &arg_iter[0];
  auto ts = &arg_iter[1];

  // result = ts + (int64)count * units_per_fixed_unit
  int64_t units_per_fixed_unit = seconds_per_unit * UnitsPerSecond(time_unit);
  auto count_i64 = ir_builder()->CreateSExt(count, i64);
  auto delta = ir_builder()->CreateMul(
      count_i64, llvm::ConstantInt::get(i64, units_per_fixed_unit));
  auto result = ir_builder()->CreateAdd(ts, delta);

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildDateArithFixed(const std::string& function_name,
                                        bool count_first,
                                        int64_t seconds_per_count,
                                        arrow::TimeUnit::type time_unit,
                                        llvm::Type* count_type) {
  auto i64 = types()->i64_type();
  llvm::Function* function;
  if (count_first) {
    function = BuildFunction(function_name, i64, {{"count", count_type}, {"ts", i64}});
  } else {
    function = BuildFunction(function_name, i64, {{"ts", i64}, {"count", count_type}});
  }
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  llvm::Value* ts = count_first ? &arg_iter[1] : &arg_iter[0];
  llvm::Value* count = count_first ? &arg_iter[0] : &arg_iter[1];

  int64_t units_per_count = seconds_per_count * UnitsPerSecond(time_unit);
  auto count_i64 = (count_type == i64) ? count : ir_builder()->CreateSExt(count, i64);
  auto delta = ir_builder()->CreateMul(
      count_i64, llvm::ConstantInt::get(i64, units_per_count));
  auto result = ir_builder()->CreateAdd(ts, delta);

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildTimestampaddCalendar(const std::string& function_name,
                                              const std::string& precompiled_millis_fn,
                                              arrow::TimeUnit::type time_unit) {
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i32 = types()->i32_type();
  auto i64 = types()->i64_type();
  auto function = BuildFunction(function_name, i64, {{"count", i32}, {"ts", i64}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  auto count = &arg_iter[0];
  auto ts = &arg_iter[1];

  int64_t upm = UnitsPerMilli(time_unit);
  auto upm_const = llvm::ConstantInt::get(i64, upm);
  auto [millis, remainder] = FloorDivRem(ts, upm_const);
  auto result_millis = ir_builder()->CreateCall(precompiled_fn, {count, millis});
  auto result_scaled = ir_builder()->CreateMul(result_millis, upm_const);
  auto result = ir_builder()->CreateAdd(result_scaled, remainder);

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildTimestampaddCalendarGeneric(
    const std::string& function_name,
    const std::string& precompiled_millis_fn,
    arrow::TimeUnit::type time_unit,
    bool count_first,
    llvm::Type* count_type) {
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  llvm::Function* function;
  if (count_first) {
    function = BuildFunction(function_name, i64, {{"count", count_type}, {"ts", i64}});
  } else {
    function = BuildFunction(function_name, i64, {{"ts", i64}, {"count", count_type}});
  }
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  llvm::Value* ts = count_first ? &arg_iter[1] : &arg_iter[0];
  llvm::Value* count = count_first ? &arg_iter[0] : &arg_iter[1];

  // Convert count to i32 if needed (precompiled fn takes int32 count for millis version)
  auto i32 = types()->i32_type();
  auto count_i32 = (count_type == i32) ? count
                                       : ir_builder()->CreateTrunc(count, i32);

  int64_t upm = UnitsPerMilli(time_unit);
  auto upm_const = llvm::ConstantInt::get(i64, upm);
  auto [millis, remainder] = FloorDivRem(ts, upm_const);

  // Precompiled millis fn always takes (int32 count, int64 millis)
  auto result_millis = ir_builder()->CreateCall(precompiled_fn, {count_i32, millis});
  auto result_scaled = ir_builder()->CreateMul(result_millis, upm_const);
  auto result = ir_builder()->CreateAdd(result_scaled, remainder);

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildExtractWrapper(const std::string& function_name,
                                        const std::string& precompiled_millis_fn,
                                        arrow::TimeUnit::type time_unit) {
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto function = BuildFunction(function_name, i64, {{"ts", i64}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto ts = &function->arg_begin()[0];
  int64_t upm = UnitsPerMilli(time_unit);
  auto millis = FloorDiv(ts, llvm::ConstantInt::get(i64, upm));
  auto result = ir_builder()->CreateCall(precompiled_fn, {millis});

  ir_builder()->CreateRet(result);

  return Status::OK();
}

Status TimestampIR::BuildTruncWrapper(const std::string& function_name,
                                      const std::string& precompiled_millis_fn,
                                      arrow::TimeUnit::type time_unit) {
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto function = BuildFunction(function_name, i64, {{"ts", i64}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto ts = &function->arg_begin()[0];
  int64_t upm = UnitsPerMilli(time_unit);
  auto upm_const = llvm::ConstantInt::get(i64, upm);
  auto millis = FloorDiv(ts, upm_const);
  auto result_millis = ir_builder()->CreateCall(precompiled_fn, {millis});
  auto result = ir_builder()->CreateMul(result_millis, upm_const);

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildDiffWrapper(const std::string& function_name,
                                     const std::string& precompiled_millis_fn,
                                     arrow::TimeUnit::type time_unit,
                                     llvm::Type* return_type) {
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto function = BuildFunction(function_name, return_type,
                                {{"ts1", i64}, {"ts2", i64}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  int64_t upm = UnitsPerMilli(time_unit);
  auto upm_const = llvm::ConstantInt::get(i64, upm);
  auto millis1 = FloorDiv(&arg_iter[0], upm_const);
  auto millis2 = FloorDiv(&arg_iter[1], upm_const);
  auto result = ir_builder()->CreateCall(precompiled_fn, {millis1, millis2});

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildCastFromTimestampWrapper(
    const std::string& function_name,
    const std::string& precompiled_millis_fn,
    arrow::TimeUnit::type time_unit,
    llvm::Type* return_type) {
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto function = BuildFunction(function_name, return_type, {{"ts", i64}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto ts = &function->arg_begin()[0];
  int64_t upm = UnitsPerMilli(time_unit);
  auto millis = FloorDiv(ts, llvm::ConstantInt::get(i64, upm));
  auto result = ir_builder()->CreateCall(precompiled_fn, {millis});

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildTimezoneWrapper(const std::string& function_name,
                                         const std::string& precompiled_millis_fn,
                                         arrow::TimeUnit::type time_unit) {
  // fn(context, ts, tz_str, tz_len) -> ts
  // Split-recombine: the timezone offset is a whole-second delta, so sub-ms survives.
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto i32 = types()->i32_type();
  auto i8ptr = llvm::PointerType::get(*context(), 0);
  auto function = BuildFunction(function_name, i64,
      {{"ctx", i64}, {"ts", i64}, {"tz", i8ptr}, {"tz_len", i32}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  auto ctx = &arg_iter[0];
  auto ts = &arg_iter[1];
  auto tz = &arg_iter[2];
  auto tz_len = &arg_iter[3];

  int64_t upm = UnitsPerMilli(time_unit);
  auto upm_const = llvm::ConstantInt::get(i64, upm);
  auto [millis, remainder] = FloorDivRem(ts, upm_const);

  auto result_millis = ir_builder()->CreateCall(precompiled_fn, {ctx, millis, tz, tz_len});
  auto result_scaled = ir_builder()->CreateMul(result_millis, upm_const);
  auto result = ir_builder()->CreateAdd(result_scaled, remainder);

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildNextDayWrapper(const std::string& function_name,
                                        const std::string& precompiled_millis_fn,
                                        arrow::TimeUnit::type time_unit) {
  // fn(context, ts, day_str, day_len) -> date64
  // next_day returns the date of the next weekday (e.g. 'MO' for Monday) at midnight,
  // so sub-ms precision is not relevant in the result — just scale the input to millis.
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto i32 = types()->i32_type();
  auto i8ptr = llvm::PointerType::get(*context(), 0);
  auto function = BuildFunction(
      function_name, i64,
      {{"ctx", i64}, {"ts", i64}, {"day", i8ptr}, {"day_len", i32}});
  auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
  ir_builder()->SetInsertPoint(entry);

  auto arg_iter = function->arg_begin();
  auto ctx = &arg_iter[0];
  auto ts = &arg_iter[1];
  auto day = &arg_iter[2];
  auto day_len = &arg_iter[3];

  int64_t upm = UnitsPerMilli(time_unit);
  auto millis = FloorDiv(ts, llvm::ConstantInt::get(i64, upm));
  auto result = ir_builder()->CreateCall(precompiled_fn, {ctx, millis, day, day_len});

  ir_builder()->CreateRet(result);
  return Status::OK();
}

Status TimestampIR::BuildCastVARCHARWrapper(const std::string& function_name,
                                            const std::string& precompiled_millis_fn,
                                            arrow::TimeUnit::type time_unit) {
  // fn(context, ts, len, &out_len) -> const char*
  // For MICRO/NANO: call the millis formatter for the first 23 chars, then append
  // sub-millisecond digits (3 for us, 6 for ns) into a new arena-allocated buffer.
  auto precompiled_fn = module()->getFunction(precompiled_millis_fn);
  if (!precompiled_fn) {
    return Status::Invalid("Precompiled function not found: ", precompiled_millis_fn);
  }

  auto i64 = types()->i64_type();
  auto i32 = types()->i32_type();
  auto i8 = llvm::Type::getInt8Ty(*context());
  auto i8ptr = llvm::PointerType::get(*context(), 0);
  auto i32ptr = llvm::PointerType::get(*context(), 0);
  auto function = BuildFunction(function_name, i8ptr,
      {{"ctx", i64}, {"ts", i64}, {"len", i64}, {"out_len", i32ptr}});

  int64_t upm = UnitsPerMilli(time_unit);
  int extra_digits = (time_unit == arrow::TimeUnit::MICRO) ? 3
                   : (time_unit == arrow::TimeUnit::NANO)  ? 6
                                                           : 0;

  if (extra_digits == 0) {
    // MILLI: pass through directly
    auto entry = llvm::BasicBlock::Create(*context(), "entry", function);
    ir_builder()->SetInsertPoint(entry);
    auto arg_iter = function->arg_begin();
    auto millis = FloorDiv(&arg_iter[1], llvm::ConstantInt::get(i64, upm));
    auto result = ir_builder()->CreateCall(
        precompiled_fn, {&arg_iter[0], millis, &arg_iter[2], &arg_iter[3]});
    ir_builder()->CreateRet(result);
    return Status::OK();
  }

  // --- MICRO or NANO path ---
  auto bb_entry = llvm::BasicBlock::Create(*context(), "entry", function);
  auto bb_append = llvm::BasicBlock::Create(*context(), "append", function);
  auto bb_done = llvm::BasicBlock::Create(*context(), "done", function);

  // Entry: call precompiled millis formatter
  ir_builder()->SetInsertPoint(bb_entry);
  auto arg_iter = function->arg_begin();
  auto ctx = &arg_iter[0];
  auto ts = &arg_iter[1];
  auto len = &arg_iter[2];
  auto out_len_ptr = &arg_iter[3];

  auto upm_const = llvm::ConstantInt::get(i64, upm);
  auto millis = FloorDiv(ts, upm_const);
  auto base_buf = ir_builder()->CreateCall(precompiled_fn, {ctx, millis, len, out_len_ptr});
  auto base_len = ir_builder()->CreateLoad(i32, out_len_ptr);

  // Check if length allows extra digits
  auto base_len_i64 = ir_builder()->CreateSExt(base_len, i64);
  auto has_room = ir_builder()->CreateICmpSLT(base_len_i64, len);
  ir_builder()->CreateCondBr(has_room, bb_append, bb_done);

  // Append: allocate new buffer, copy prefix, write sub-ms digits
  ir_builder()->SetInsertPoint(bb_append);

  auto extra_const = llvm::ConstantInt::get(i32, extra_digits);
  auto full_len = ir_builder()->CreateAdd(base_len, extra_const);

  // Clamp to len
  auto len_i32 = ir_builder()->CreateTrunc(len, i32);
  auto clamped_len = ir_builder()->CreateSelect(
      ir_builder()->CreateICmpSLT(full_len, len_i32), full_len, len_i32);

  // arena_malloc(ctx, clamped_len)
  auto arena_fn = module()->getFunction("gdv_fn_context_arena_malloc");
  auto new_buf = ir_builder()->CreateCall(arena_fn, {ctx, clamped_len});

  // memcpy(new_buf, base_buf, base_len)
  ir_builder()->CreateMemCpy(
      new_buf, llvm::MaybeAlign(1), base_buf, llvm::MaybeAlign(1), base_len);

  // Compute the non-negative sub-ms remainder consistent with floor division.
  // FloorDivRem guarantees remainder is in [0, upm), even for negative timestamps.
  auto [millis_dup, sub_ms_rem] = FloorDivRem(ts, upm_const);
  (void)millis_dup;
  auto abs_rem = sub_ms_rem;

  // Write digits from most significant to least significant
  // For MICRO: divisors are 100, 10, 1
  // For NANO:  divisors are 100000, 10000, 1000, 100, 10, 1
  int64_t divisor = 1;
  for (int i = 0; i < extra_digits - 1; ++i) divisor *= 10;

  llvm::BasicBlock* last_append_bb = nullptr;
  for (int i = 0; i < extra_digits; ++i) {
    auto idx = ir_builder()->CreateAdd(base_len,
        llvm::ConstantInt::get(i32, i));
    auto write_pos = ir_builder()->CreateICmpSLT(idx, clamped_len);

    // digit = (abs_rem / divisor) % 10 + '0'
    auto d = ir_builder()->CreateSDiv(abs_rem,
        llvm::ConstantInt::get(i64, divisor));
    auto digit = ir_builder()->CreateSRem(d, llvm::ConstantInt::get(i64, 10));
    auto ch = ir_builder()->CreateAdd(
        ir_builder()->CreateTrunc(digit, i8), llvm::ConstantInt::get(i8, '0'));

    auto gep = ir_builder()->CreateGEP(i8, new_buf, idx);

    auto bb_store = llvm::BasicBlock::Create(*context(), "store", function);
    auto bb_next = llvm::BasicBlock::Create(*context(), "next", function);
    ir_builder()->CreateCondBr(write_pos, bb_store, bb_next);

    ir_builder()->SetInsertPoint(bb_store);
    ir_builder()->CreateStore(ch, gep);
    ir_builder()->CreateBr(bb_next);

    ir_builder()->SetInsertPoint(bb_next);
    last_append_bb = bb_next;
    divisor /= 10;
  }

  // Store final out_len
  ir_builder()->CreateStore(clamped_len, out_len_ptr);
  ir_builder()->CreateBr(bb_done);

  // Done: phi to select return value
  ir_builder()->SetInsertPoint(bb_done);
  auto phi = ir_builder()->CreatePHI(i8ptr, 2, "result");
  phi->addIncoming(base_buf, bb_entry);
  phi->addIncoming(new_buf, last_append_bb);

  ir_builder()->CreateRet(phi);
  return Status::OK();
}

llvm::Value* TimestampIR::FloorDiv(llvm::Value* ts, llvm::Value* divisor) {
  auto i64 = types()->i64_type();
  auto zero = llvm::ConstantInt::get(i64, 0);
  auto one = llvm::ConstantInt::get(i64, 1);
  auto quotient = ir_builder()->CreateSDiv(ts, divisor);
  auto remainder = ir_builder()->CreateSRem(ts, divisor);
  auto has_remainder = ir_builder()->CreateICmpNE(remainder, zero);
  auto is_negative = ir_builder()->CreateICmpSLT(ts, zero);
  auto needs_adjust = ir_builder()->CreateAnd(is_negative, has_remainder);
  return ir_builder()->CreateSub(quotient,
                                 ir_builder()->CreateSelect(needs_adjust, one, zero));
}

std::pair<llvm::Value*, llvm::Value*> TimestampIR::FloorDivRem(
    llvm::Value* ts, llvm::Value* divisor) {
  auto i64 = types()->i64_type();
  auto zero = llvm::ConstantInt::get(i64, 0);
  auto one = llvm::ConstantInt::get(i64, 1);
  auto quotient = ir_builder()->CreateSDiv(ts, divisor);
  auto remainder = ir_builder()->CreateSRem(ts, divisor);
  auto has_remainder = ir_builder()->CreateICmpNE(remainder, zero);
  auto is_negative = ir_builder()->CreateICmpSLT(ts, zero);
  auto needs_adjust = ir_builder()->CreateAnd(is_negative, has_remainder);
  auto adj = ir_builder()->CreateSelect(needs_adjust, one, zero);
  auto floor_q = ir_builder()->CreateSub(quotient, adj);
  auto floor_r = ir_builder()->CreateAdd(
      remainder, ir_builder()->CreateSelect(needs_adjust, divisor, zero));
  return {floor_q, floor_r};
}

/*static*/ Status TimestampIR::AddFunctions(Engine* engine) {
  auto ts_ir = std::make_shared<TimestampIR>(engine);
  auto i32 = ts_ir->types()->i32_type();
  auto i64 = ts_ir->types()->i64_type();
  auto f64 = ts_ir->types()->double_type();

  for (auto unit : kUnits) {
    auto sfx = UnitSuffix(unit);

    // Helper: skip functions whose precompiled base is missing, but warn on
    // unexpected errors (type mismatch, LLVM failure, etc.).
    auto try_build = [](const std::string& ir_name, Status status) {
      if (status.ok() || status.IsInvalid()) {
        return;  // OK or precompiled function not found — expected
      }
      ARROW_LOG(DEBUG) << "TimestampIR: unexpected error building " << ir_name
                       << ": " << status.ToString();
    };

    // Fixed-unit: pure IR (always succeeds)
    for (const auto& fa : kFixedAdds) {
      auto ir_name = std::string(fa.name) + "_int32_timestamp" + sfx;
      ARROW_RETURN_NOT_OK(ts_ir->BuildTimestampaddFixed(ir_name, fa.seconds, unit));
    }

    // Calendar-based: precompiled wrapper
    for (const auto* ca : kCalendarAdds) {
      auto ir_name = std::string(ca) + "_int32_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildTimestampaddCalendar(ir_name,
                       std::string(ca) + "_int32_timestamp", unit));
    }

    // Extract functions
    for (const auto* ex : kExtracts) {
      auto ir_name = std::string(ex) + "_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildExtractWrapper(ir_name,
                       std::string(ex) + "_timestamp", unit));
    }

    // date_trunc functions
    for (const auto* tr : kTruncs) {
      auto ir_name = std::string(tr) + "_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildTruncWrapper(ir_name,
                       std::string(tr) + "_timestamp", unit));
    }

    // timestampdiff functions (two ts -> int32)
    for (const auto* di : kDiffs) {
      auto ir_name = std::string(di) + "_timestamp_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildDiffWrapper(ir_name,
                       std::string(di) + "_timestamp_timestamp", unit, i32));
    }

    // months_between / datediff
    for (const auto& ts2 : kTwoTsScalars) {
      auto ir_name = std::string(ts2.name) + "_timestamp_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildDiffWrapper(ir_name,
                       std::string(ts2.name) + "_timestamp_timestamp", unit,
                       ts2.returns_float ? f64 : i32));
    }

    // Cast from timestamp
    for (const auto& c : kCastsFromTs) {
      auto ir_name = std::string(c.name) + "_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildCastFromTimestampWrapper(ir_name,
                       std::string(c.name) + "_timestamp", unit,
                       c.returns_i32 ? i32 : i64));
    }

    // date_add/add/date_sub/subtract/date_diff with int32 and int64
    for (const auto& da : kDateArithEntries) {
      for (auto* count_type : {i32, i64}) {
        const char* type_name = (count_type == i32) ? "int32" : "int64";
        std::string ir_name;
        if (da.count_first) {
          ir_name = std::string(da.name) + "_" + type_name + "_timestamp" + sfx;
        } else {
          ir_name = std::string(da.name) + "_timestamp_" + type_name + sfx;
        }
        try_build(ir_name,
                     ts_ir->BuildDateArithFixed(ir_name, da.count_first,
                         da.sign * 86400LL, unit, count_type));
      }
    }

    // int64 variants of timestampadd (pure IR arithmetic)
    for (const auto& fa : kFixedAdds) {
      auto ir_name = std::string(fa.name) + "_int64_timestamp" + sfx;
      try_build(ir_name,
                   ts_ir->BuildDateArithFixed(ir_name, /*count_first=*/true,
                       fa.seconds, unit, i64));
    }

    // Reversed-arg variants: timestampaddX(timestamp, int32/int64) -> timestamp
    for (const auto& fa : kFixedAdds) {
      auto ir32 = std::string(fa.name) + "_timestamp_int32" + sfx;
      try_build(ir32,
                   ts_ir->BuildDateArithFixed(ir32, /*count_first=*/false,
                       fa.seconds, unit, i32));
      auto ir64 = std::string(fa.name) + "_timestamp_int64" + sfx;
      try_build(ir64,
                   ts_ir->BuildDateArithFixed(ir64, /*count_first=*/false,
                       fa.seconds, unit, i64));
    }

    // Reversed-arg calendar: timestampaddMonth/Quarter/Year(timestamp, int32/int64)
    // and int64 calendar: timestampaddMonth/Quarter/Year(int64, timestamp)
    // All use the precompiled (int32, timestamp) millis function as the base.
    for (const auto* ca : kCalendarAdds) {
      auto millis_fn = std::string(ca) + "_int32_timestamp";
      // (timestamp, int32) variant
      auto rev32 = std::string(ca) + "_timestamp_int32" + sfx;
      try_build(rev32,
                   ts_ir->BuildTimestampaddCalendarGeneric(rev32, millis_fn, unit,
                       /*count_first=*/false, i32));
      // (timestamp, int64) variant
      auto rev64 = std::string(ca) + "_timestamp_int64" + sfx;
      try_build(rev64,
                   ts_ir->BuildTimestampaddCalendarGeneric(rev64, millis_fn, unit,
                       /*count_first=*/false, i64));
      // (int64, timestamp) variant
      auto fwd64 = std::string(ca) + "_int64_timestamp" + sfx;
      try_build(fwd64,
                   ts_ir->BuildTimestampaddCalendarGeneric(fwd64, millis_fn, unit,
                       /*count_first=*/true, i64));
    }

    // Timezone functions: to_utc/from_utc (split-recombine)
    {
      std::string ir_to = std::string("to_utc_timezone_timestamp") + sfx;
      try_build(ir_to,
                   ts_ir->BuildTimezoneWrapper(ir_to, "to_utc_timezone_timestamp", unit));
      std::string ir_from = std::string("from_utc_timezone_timestamp") + sfx;
      try_build(ir_from,
                   ts_ir->BuildTimezoneWrapper(ir_from, "from_utc_timezone_timestamp",
                                               unit));
    }

    // castVARCHAR(timestamp, int64): scale to millis
    {
      std::string ir_name = std::string("castVARCHAR_timestamp_int64") + sfx;
      try_build(ir_name,
                   ts_ir->BuildCastVARCHARWrapper(ir_name, "castVARCHAR_timestamp_int64",
                                                   unit));
    }

    // next_day(timestamp, utf8): scale to millis, return date64
    {
      std::string ir_name = std::string("next_day_from_timestamp") + sfx;
      try_build(ir_name,
                ts_ir->BuildNextDayWrapper(ir_name, "next_day_from_timestamp", unit));
    }
  }

  // Validate that the set of functions we tried to build matches AllFunctionNames().
  // This catches drift between AddFunctions() and BuildAllFunctionNames().
  const auto& expected = AllFunctionNames();
  for (const auto& name : expected) {
    if (!ts_ir->module()->getFunction(name)) {
      ARROW_LOG(DEBUG) << "TimestampIR: " << name
                       << " in AllFunctionNames() but not created (precompiled base "
                          "likely missing — OK if intentional)";
    }
  }
  for (auto& fn : *ts_ir->module()) {
    auto name = fn.getName().str();
    // Only check functions with unit suffixes that we generate
    if ((name.find("_us") != std::string::npos || name.find("_ns") != std::string::npos) &&
        expected.find(name) == expected.end()) {
      ARROW_LOG(WARNING) << "TimestampIR: function " << name
                         << " was created but is not in AllFunctionNames() — "
                            "it will not be remapped during code generation";
    }
  }

  return Status::OK();
}

}  // namespace gandiva
