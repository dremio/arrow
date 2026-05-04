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

// Precompiled _us and _ns wrappers for timestamp functions.
//
// Each function scales a microsecond or nanosecond timestamp to milliseconds
// (using floor division to handle negative timestamps correctly), calls the
// existing precompiled millisecond-scale function, then scales the result back.
//
// This replaces the dynamic LLVM IR builders in timestamp_ir.cc, which built
// the same wrappers programmatically at JIT initialization time.

#include <stdint.h>
#include <string.h>

#include "./types.h"

// Forward declarations for precompiled milli-scale functions not in types.h.
extern "C" {

gdv_int64 extractEpoch_timestamp(gdv_timestamp millis);

gdv_int64 date_trunc_Millennium_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Century_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Decade_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Year_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Quarter_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Month_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Day_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Hour_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Minute_timestamp(gdv_timestamp millis);
gdv_int64 date_trunc_Second_timestamp(gdv_timestamp millis);

gdv_int32 timestampdiffSecond_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);
gdv_int32 timestampdiffMinute_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);
gdv_int32 timestampdiffHour_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);
gdv_int32 timestampdiffDay_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);
gdv_int32 timestampdiffWeek_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);
gdv_int32 timestampdiffQuarter_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);
gdv_int32 timestampdiffYear_timestamp_timestamp(gdv_timestamp ts1, gdv_timestamp ts2);

gdv_int64 to_utc_timezone_timestamp(gdv_int64 ctx, gdv_int64 millis, const char* tz,
                                    gdv_int32 tz_len);
gdv_int64 from_utc_timezone_timestamp(gdv_int64 ctx, gdv_int64 millis, const char* tz,
                                      gdv_int32 tz_len);

}  // end forward declarations

// Floor division: rounds toward -inf (unlike C's truncation-toward-zero).
// e.g., ts_floor_div(-1001, 1000) == -2, not -1.
static FORCE_INLINE gdv_int64 ts_floor_div(gdv_int64 ts, gdv_int64 divisor) {
  gdv_int64 q = ts / divisor;
  gdv_int64 r = ts % divisor;
  return (ts < 0 && r != 0) ? q - 1 : q;
}

// Floor remainder: always non-negative, satisfies ts = floor_div*divisor + floor_rem.
static FORCE_INLINE gdv_int64 ts_floor_rem(gdv_int64 ts, gdv_int64 divisor) {
  gdv_int64 r = ts % divisor;
  return (ts < 0 && r != 0) ? r + divisor : r;
}

extern "C" {

// ─────────────────────────────────────────────────────────────────────────────
// MICROSECOND variants  (units per millisecond = 1000)
// ─────────────────────────────────────────────────────────────────────────────

// Fixed-unit timestampadd: ts ± count * constant (pure arithmetic).
// Four arg-order variants per base name: (int32,ts), (int64,ts), (ts,int32), (ts,int64).
#define FIXED_ADD_US(FN, UNITS_PER_SECOND)                                              \
  FORCE_INLINE gdv_int64 FN##_int32_timestamp_us(gdv_int32 cnt, gdv_timestamp ts) {    \
    return ts + (gdv_int64)cnt * ((UNITS_PER_SECOND)*1000000LL);                        \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_int64_timestamp_us(gdv_int64 cnt, gdv_timestamp ts) {    \
    return ts + cnt * ((UNITS_PER_SECOND)*1000000LL);                                   \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {    \
    return ts + (gdv_int64)cnt * ((UNITS_PER_SECOND)*1000000LL);                        \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {    \
    return ts + cnt * ((UNITS_PER_SECOND)*1000000LL);                                   \
  }

FIXED_ADD_US(timestampaddSecond, 1)
FIXED_ADD_US(timestampaddMinute, 60)
FIXED_ADD_US(timestampaddHour, 3600)
FIXED_ADD_US(timestampaddDay, 86400)
FIXED_ADD_US(timestampaddWeek, 604800)

// Calendar-based timestampadd: floor-split ts into (millis, sub-ms remainder),
// call the precompiled milli function, then reassemble.
// All four arg-order variants share the same (int32, millis) base function.
#define CALENDAR_ADD_US(FN)                                                             \
  FORCE_INLINE gdv_int64 FN##_int32_timestamp_us(gdv_int32 cnt, gdv_timestamp ts) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000LL);                                            \
    gdv_int64 rem = ts_floor_rem(ts, 1000LL);                                           \
    return FN##_int32_timestamp(cnt, ms) * 1000LL + rem;                                \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000LL);                                            \
    gdv_int64 rem = ts_floor_rem(ts, 1000LL);                                           \
    return FN##_int32_timestamp(cnt, ms) * 1000LL + rem;                                \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_int64_timestamp_us(gdv_int64 cnt, gdv_timestamp ts) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000LL);                                            \
    gdv_int64 rem = ts_floor_rem(ts, 1000LL);                                           \
    return FN##_int32_timestamp((gdv_int32)cnt, ms) * 1000LL + rem;                    \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000LL);                                            \
    gdv_int64 rem = ts_floor_rem(ts, 1000LL);                                           \
    return FN##_int32_timestamp((gdv_int32)cnt, ms) * 1000LL + rem;                    \
  }

CALENDAR_ADD_US(timestampaddMonth)
CALENDAR_ADD_US(timestampaddQuarter)
CALENDAR_ADD_US(timestampaddYear)

// Extract: scale ts to millis, call precompiled extractor, return scalar.
#define EXTRACT_US(FN)                                                                  \
  FORCE_INLINE gdv_int64 FN##_timestamp_us(gdv_timestamp ts) {                         \
    return FN##_timestamp(ts_floor_div(ts, 1000LL));                                    \
  }

EXTRACT_US(extractMillennium)
EXTRACT_US(extractCentury)
EXTRACT_US(extractDecade)
EXTRACT_US(extractYear)
EXTRACT_US(extractQuarter)
EXTRACT_US(extractMonth)
EXTRACT_US(extractWeek)
EXTRACT_US(extractDay)
EXTRACT_US(extractHour)
EXTRACT_US(extractMinute)
EXTRACT_US(extractSecond)
EXTRACT_US(extractDoy)
EXTRACT_US(extractDow)
EXTRACT_US(extractEpoch)

// date_trunc: scale to millis, truncate, scale back (sub-ms remainder is zeroed).
#define TRUNC_US(FN)                                                                    \
  FORCE_INLINE gdv_int64 FN##_timestamp_us(gdv_timestamp ts) {                         \
    return FN##_timestamp(ts_floor_div(ts, 1000LL)) * 1000LL;                          \
  }

TRUNC_US(date_trunc_Millennium)
TRUNC_US(date_trunc_Century)
TRUNC_US(date_trunc_Decade)
TRUNC_US(date_trunc_Year)
TRUNC_US(date_trunc_Quarter)
TRUNC_US(date_trunc_Month)
TRUNC_US(date_trunc_Week)
TRUNC_US(date_trunc_Day)
TRUNC_US(date_trunc_Hour)
TRUNC_US(date_trunc_Minute)
TRUNC_US(date_trunc_Second)

// timestampdiff: scale both inputs to millis, call precompiled, return int32.
#define DIFF_US(FN)                                                                     \
  FORCE_INLINE gdv_int32 FN##_timestamp_timestamp_us(gdv_timestamp ts1,                \
                                                      gdv_timestamp ts2) {              \
    return FN##_timestamp_timestamp(ts_floor_div(ts1, 1000LL),                         \
                                    ts_floor_div(ts2, 1000LL));                         \
  }

DIFF_US(timestampdiffSecond)
DIFF_US(timestampdiffMinute)
DIFF_US(timestampdiffHour)
DIFF_US(timestampdiffDay)
DIFF_US(timestampdiffWeek)
DIFF_US(timestampdiffMonth)
DIFF_US(timestampdiffQuarter)
DIFF_US(timestampdiffYear)

FORCE_INLINE gdv_float64
months_between_timestamp_timestamp_us(gdv_timestamp ts1, gdv_timestamp ts2) {
  return months_between_timestamp_timestamp((gdv_uint64)ts_floor_div(ts1, 1000LL),
                                            (gdv_uint64)ts_floor_div(ts2, 1000LL));
}

FORCE_INLINE gdv_int32
datediff_timestamp_timestamp_us(gdv_timestamp ts1, gdv_timestamp ts2) {
  return datediff_timestamp_timestamp(ts_floor_div(ts1, 1000LL),
                                      ts_floor_div(ts2, 1000LL));
}

// Cast from timestamp (scale to millis, call precompiled cast).
FORCE_INLINE gdv_date64 castDATE_timestamp_us(gdv_timestamp ts) {
  return castDATE_timestamp(ts_floor_div(ts, 1000LL));
}
FORCE_INLINE gdv_time32 castTIME_timestamp_us(gdv_timestamp ts) {
  return castTIME_timestamp(ts_floor_div(ts, 1000LL));
}
FORCE_INLINE gdv_date64 last_day_from_timestamp_us(gdv_timestamp ts) {
  return last_day_from_timestamp(ts_floor_div(ts, 1000LL));
}

// Date arithmetic: add or subtract whole days (pure arithmetic, 86400 s/day).
// Positive delta for add, negative for sub.
FORCE_INLINE gdv_int64 date_add_int32_timestamp_us(gdv_int32 cnt, gdv_timestamp ts) {
  return ts + (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_add_int64_timestamp_us(gdv_int64 cnt, gdv_timestamp ts) {
  return ts + cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 add_int32_timestamp_us(gdv_int32 cnt, gdv_timestamp ts) {
  return ts + (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 add_int64_timestamp_us(gdv_int64 cnt, gdv_timestamp ts) {
  return ts + cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_add_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {
  return ts + (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_add_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {
  return ts + cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 add_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {
  return ts + (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 add_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {
  return ts + cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_sub_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {
  return ts - (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_sub_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {
  return ts - cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 subtract_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {
  return ts - (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 subtract_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {
  return ts - cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_diff_timestamp_int32_us(gdv_timestamp ts, gdv_int32 cnt) {
  return ts - (gdv_int64)cnt * 86400000000LL;
}
FORCE_INLINE gdv_int64 date_diff_timestamp_int64_us(gdv_timestamp ts, gdv_int64 cnt) {
  return ts - cnt * 86400000000LL;
}

// Timezone: floor-split ts, apply milli-scale tz conversion, reassemble.
// The UTC offset is a whole-second delta so sub-ms precision survives unchanged.
FORCE_INLINE gdv_int64 to_utc_timezone_timestamp_us(gdv_int64 ctx, gdv_int64 ts,
                                                     const char* tz, gdv_int32 tz_len) {
  gdv_int64 ms = ts_floor_div(ts, 1000LL);
  gdv_int64 rem = ts_floor_rem(ts, 1000LL);
  return to_utc_timezone_timestamp(ctx, ms, tz, tz_len) * 1000LL + rem;
}

FORCE_INLINE gdv_int64 from_utc_timezone_timestamp_us(gdv_int64 ctx, gdv_int64 ts,
                                                       const char* tz,
                                                       gdv_int32 tz_len) {
  gdv_int64 ms = ts_floor_div(ts, 1000LL);
  gdv_int64 rem = ts_floor_rem(ts, 1000LL);
  return from_utc_timezone_timestamp(ctx, ms, tz, tz_len) * 1000LL + rem;
}

// castVARCHAR(timestamp_us, int64): call the milli formatter for the base string,
// then append 3 sub-millisecond digits (the microseconds-within-the-millisecond).
const char* castVARCHAR_timestamp_int64_us(gdv_int64 ctx, gdv_timestamp ts,
                                           gdv_int64 len, gdv_int32* out_len) {
  gdv_int64 ms = ts_floor_div(ts, 1000LL);
  const char* base_buf = castVARCHAR_timestamp_int64(ctx, ms, len, out_len);
  gdv_int32 base_len = *out_len;
  if ((gdv_int64)base_len >= len) return base_buf;

  gdv_int32 full_len = base_len + 3;
  gdv_int32 clamped_len = (full_len < (gdv_int32)len) ? full_len : (gdv_int32)len;
  char* new_buf =
      reinterpret_cast<char*>(gdv_fn_context_arena_malloc(ctx, clamped_len));
  memcpy(new_buf, base_buf, base_len);

  gdv_int64 sub = ts_floor_rem(ts, 1000LL);  // microseconds within the millisecond [0,999]
  if (base_len + 0 < clamped_len) new_buf[base_len + 0] = '0' + (char)((sub / 100) % 10);
  if (base_len + 1 < clamped_len) new_buf[base_len + 1] = '0' + (char)((sub / 10) % 10);
  if (base_len + 2 < clamped_len) new_buf[base_len + 2] = '0' + (char)(sub % 10);

  *out_len = clamped_len;
  return new_buf;
}

// next_day(timestamp_us, string): scale to millis, call precompiled, return date64.
FORCE_INLINE gdv_int64 next_day_from_timestamp_us(gdv_int64 ctx, gdv_timestamp ts,
                                                   const char* day, gdv_int32 day_len) {
  return next_day_from_timestamp(ctx, ts_floor_div(ts, 1000LL), day, day_len);
}

// ─────────────────────────────────────────────────────────────────────────────
// NANOSECOND variants  (units per millisecond = 1000000)
// ─────────────────────────────────────────────────────────────────────────────

#define FIXED_ADD_NS(FN, UNITS_PER_SECOND)                                              \
  FORCE_INLINE gdv_int64 FN##_int32_timestamp_ns(gdv_int32 cnt, gdv_timestamp ts) {    \
    return ts + (gdv_int64)cnt * ((UNITS_PER_SECOND)*1000000000LL);                     \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_int64_timestamp_ns(gdv_int64 cnt, gdv_timestamp ts) {    \
    return ts + cnt * ((UNITS_PER_SECOND)*1000000000LL);                                \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {    \
    return ts + (gdv_int64)cnt * ((UNITS_PER_SECOND)*1000000000LL);                     \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {    \
    return ts + cnt * ((UNITS_PER_SECOND)*1000000000LL);                                \
  }

FIXED_ADD_NS(timestampaddSecond, 1)
FIXED_ADD_NS(timestampaddMinute, 60)
FIXED_ADD_NS(timestampaddHour, 3600)
FIXED_ADD_NS(timestampaddDay, 86400)
FIXED_ADD_NS(timestampaddWeek, 604800)

#define CALENDAR_ADD_NS(FN)                                                             \
  FORCE_INLINE gdv_int64 FN##_int32_timestamp_ns(gdv_int32 cnt, gdv_timestamp ts) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000000LL);                                         \
    gdv_int64 rem = ts_floor_rem(ts, 1000000LL);                                        \
    return FN##_int32_timestamp(cnt, ms) * 1000000LL + rem;                             \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000000LL);                                         \
    gdv_int64 rem = ts_floor_rem(ts, 1000000LL);                                        \
    return FN##_int32_timestamp(cnt, ms) * 1000000LL + rem;                             \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_int64_timestamp_ns(gdv_int64 cnt, gdv_timestamp ts) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000000LL);                                         \
    gdv_int64 rem = ts_floor_rem(ts, 1000000LL);                                        \
    return FN##_int32_timestamp((gdv_int32)cnt, ms) * 1000000LL + rem;                 \
  }                                                                                     \
  FORCE_INLINE gdv_int64 FN##_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {    \
    gdv_int64 ms = ts_floor_div(ts, 1000000LL);                                         \
    gdv_int64 rem = ts_floor_rem(ts, 1000000LL);                                        \
    return FN##_int32_timestamp((gdv_int32)cnt, ms) * 1000000LL + rem;                 \
  }

CALENDAR_ADD_NS(timestampaddMonth)
CALENDAR_ADD_NS(timestampaddQuarter)
CALENDAR_ADD_NS(timestampaddYear)

#define EXTRACT_NS(FN)                                                                  \
  FORCE_INLINE gdv_int64 FN##_timestamp_ns(gdv_timestamp ts) {                         \
    return FN##_timestamp(ts_floor_div(ts, 1000000LL));                                 \
  }

EXTRACT_NS(extractMillennium)
EXTRACT_NS(extractCentury)
EXTRACT_NS(extractDecade)
EXTRACT_NS(extractYear)
EXTRACT_NS(extractQuarter)
EXTRACT_NS(extractMonth)
EXTRACT_NS(extractWeek)
EXTRACT_NS(extractDay)
EXTRACT_NS(extractHour)
EXTRACT_NS(extractMinute)
EXTRACT_NS(extractSecond)
EXTRACT_NS(extractDoy)
EXTRACT_NS(extractDow)
EXTRACT_NS(extractEpoch)

#define TRUNC_NS(FN)                                                                    \
  FORCE_INLINE gdv_int64 FN##_timestamp_ns(gdv_timestamp ts) {                         \
    return FN##_timestamp(ts_floor_div(ts, 1000000LL)) * 1000000LL;                    \
  }

TRUNC_NS(date_trunc_Millennium)
TRUNC_NS(date_trunc_Century)
TRUNC_NS(date_trunc_Decade)
TRUNC_NS(date_trunc_Year)
TRUNC_NS(date_trunc_Quarter)
TRUNC_NS(date_trunc_Month)
TRUNC_NS(date_trunc_Week)
TRUNC_NS(date_trunc_Day)
TRUNC_NS(date_trunc_Hour)
TRUNC_NS(date_trunc_Minute)
TRUNC_NS(date_trunc_Second)

#define DIFF_NS(FN)                                                                     \
  FORCE_INLINE gdv_int32 FN##_timestamp_timestamp_ns(gdv_timestamp ts1,                \
                                                      gdv_timestamp ts2) {              \
    return FN##_timestamp_timestamp(ts_floor_div(ts1, 1000000LL),                      \
                                    ts_floor_div(ts2, 1000000LL));                      \
  }

DIFF_NS(timestampdiffSecond)
DIFF_NS(timestampdiffMinute)
DIFF_NS(timestampdiffHour)
DIFF_NS(timestampdiffDay)
DIFF_NS(timestampdiffWeek)
DIFF_NS(timestampdiffMonth)
DIFF_NS(timestampdiffQuarter)
DIFF_NS(timestampdiffYear)

FORCE_INLINE gdv_float64
months_between_timestamp_timestamp_ns(gdv_timestamp ts1, gdv_timestamp ts2) {
  return months_between_timestamp_timestamp((gdv_uint64)ts_floor_div(ts1, 1000000LL),
                                            (gdv_uint64)ts_floor_div(ts2, 1000000LL));
}

FORCE_INLINE gdv_int32
datediff_timestamp_timestamp_ns(gdv_timestamp ts1, gdv_timestamp ts2) {
  return datediff_timestamp_timestamp(ts_floor_div(ts1, 1000000LL),
                                      ts_floor_div(ts2, 1000000LL));
}

FORCE_INLINE gdv_date64 castDATE_timestamp_ns(gdv_timestamp ts) {
  return castDATE_timestamp(ts_floor_div(ts, 1000000LL));
}
FORCE_INLINE gdv_time32 castTIME_timestamp_ns(gdv_timestamp ts) {
  return castTIME_timestamp(ts_floor_div(ts, 1000000LL));
}
FORCE_INLINE gdv_date64 last_day_from_timestamp_ns(gdv_timestamp ts) {
  return last_day_from_timestamp(ts_floor_div(ts, 1000000LL));
}

FORCE_INLINE gdv_int64 date_add_int32_timestamp_ns(gdv_int32 cnt, gdv_timestamp ts) {
  return ts + (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_add_int64_timestamp_ns(gdv_int64 cnt, gdv_timestamp ts) {
  return ts + cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 add_int32_timestamp_ns(gdv_int32 cnt, gdv_timestamp ts) {
  return ts + (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 add_int64_timestamp_ns(gdv_int64 cnt, gdv_timestamp ts) {
  return ts + cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_add_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {
  return ts + (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_add_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {
  return ts + cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 add_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {
  return ts + (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 add_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {
  return ts + cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_sub_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {
  return ts - (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_sub_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {
  return ts - cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 subtract_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {
  return ts - (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 subtract_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {
  return ts - cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_diff_timestamp_int32_ns(gdv_timestamp ts, gdv_int32 cnt) {
  return ts - (gdv_int64)cnt * 86400000000000LL;
}
FORCE_INLINE gdv_int64 date_diff_timestamp_int64_ns(gdv_timestamp ts, gdv_int64 cnt) {
  return ts - cnt * 86400000000000LL;
}

FORCE_INLINE gdv_int64 to_utc_timezone_timestamp_ns(gdv_int64 ctx, gdv_int64 ts,
                                                     const char* tz, gdv_int32 tz_len) {
  gdv_int64 ms = ts_floor_div(ts, 1000000LL);
  gdv_int64 rem = ts_floor_rem(ts, 1000000LL);
  return to_utc_timezone_timestamp(ctx, ms, tz, tz_len) * 1000000LL + rem;
}

FORCE_INLINE gdv_int64 from_utc_timezone_timestamp_ns(gdv_int64 ctx, gdv_int64 ts,
                                                       const char* tz,
                                                       gdv_int32 tz_len) {
  gdv_int64 ms = ts_floor_div(ts, 1000000LL);
  gdv_int64 rem = ts_floor_rem(ts, 1000000LL);
  return from_utc_timezone_timestamp(ctx, ms, tz, tz_len) * 1000000LL + rem;
}

// castVARCHAR(timestamp_ns, int64): append 6 sub-millisecond digits.
const char* castVARCHAR_timestamp_int64_ns(gdv_int64 ctx, gdv_timestamp ts,
                                           gdv_int64 len, gdv_int32* out_len) {
  gdv_int64 ms = ts_floor_div(ts, 1000000LL);
  const char* base_buf = castVARCHAR_timestamp_int64(ctx, ms, len, out_len);
  gdv_int32 base_len = *out_len;
  if ((gdv_int64)base_len >= len) return base_buf;

  gdv_int32 full_len = base_len + 6;
  gdv_int32 clamped_len = (full_len < (gdv_int32)len) ? full_len : (gdv_int32)len;
  char* new_buf =
      reinterpret_cast<char*>(gdv_fn_context_arena_malloc(ctx, clamped_len));
  memcpy(new_buf, base_buf, base_len);

  gdv_int64 sub = ts_floor_rem(ts, 1000000LL);  // nanoseconds within the millisecond [0,999999]
  if (base_len + 0 < clamped_len) new_buf[base_len + 0] = '0' + (char)((sub / 100000) % 10);
  if (base_len + 1 < clamped_len) new_buf[base_len + 1] = '0' + (char)((sub / 10000) % 10);
  if (base_len + 2 < clamped_len) new_buf[base_len + 2] = '0' + (char)((sub / 1000) % 10);
  if (base_len + 3 < clamped_len) new_buf[base_len + 3] = '0' + (char)((sub / 100) % 10);
  if (base_len + 4 < clamped_len) new_buf[base_len + 4] = '0' + (char)((sub / 10) % 10);
  if (base_len + 5 < clamped_len) new_buf[base_len + 5] = '0' + (char)(sub % 10);

  *out_len = clamped_len;
  return new_buf;
}

FORCE_INLINE gdv_int64 next_day_from_timestamp_ns(gdv_int64 ctx, gdv_timestamp ts,
                                                   const char* day, gdv_int32 day_len) {
  return next_day_from_timestamp(ctx, ts_floor_div(ts, 1000000LL), day, day_len);
}

}  // extern "C"
