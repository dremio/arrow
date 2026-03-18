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

#include <ctime>

#include <gtest/gtest.h>
#include "./epoch_time_point.h"
#include "gandiva/precompiled/testing.h"
#include "gandiva/precompiled/types.h"

#include "gandiva/date_utils.h"

namespace gandiva {

TEST(TestEpochTimePoint, TestTm) {
  auto ts = StringToTimestamp("2015-05-07 10:20:34");
  EpochTimePoint tp(ts);

  struct tm* tm_ptr;
#if defined(_WIN32)
  __time64_t tsec = ts / 1000;
  tm_ptr = _gmtime64(&tsec);
#else
  struct tm tm;
  time_t tsec = ts / 1000;
  tm_ptr = gmtime_r(&tsec, &tm);
#endif

  EXPECT_EQ(tp.TmYear(), tm_ptr->tm_year);
  EXPECT_EQ(tp.TmMon(), tm_ptr->tm_mon);
  EXPECT_EQ(tp.TmYday(), tm_ptr->tm_yday);
  EXPECT_EQ(tp.TmMday(), tm_ptr->tm_mday);
  EXPECT_EQ(tp.TmWday(), tm_ptr->tm_wday);
  EXPECT_EQ(tp.TmHour(), tm_ptr->tm_hour);
  EXPECT_EQ(tp.TmMin(), tm_ptr->tm_min);
  EXPECT_EQ(tp.TmSec(), tm_ptr->tm_sec);
}

TEST(TestEpochTimePoint, TestAddYears) {
  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddYears(2),
            EpochTimePoint(StringToTimestamp("2017-05-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddYears(0),
            EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddYears(-1),
            EpochTimePoint(StringToTimestamp("2014-05-05 10:20:34")));
}

TEST(TestEpochTimePoint, TestAddMonths) {
  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddMonths(2),
            EpochTimePoint(StringToTimestamp("2015-07-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddMonths(11),
            EpochTimePoint(StringToTimestamp("2016-04-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddMonths(0),
            EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddMonths(-1),
            EpochTimePoint(StringToTimestamp("2015-04-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddMonths(-10),
            EpochTimePoint(StringToTimestamp("2014-07-05 10:20:34")));
}

TEST(TestEpochTimePoint, TestAddDays) {
  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddDays(2),
            EpochTimePoint(StringToTimestamp("2015-05-07 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddDays(11),
            EpochTimePoint(StringToTimestamp("2015-05-16 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddDays(0),
            EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddDays(-1),
            EpochTimePoint(StringToTimestamp("2015-05-04 10:20:34")));

  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).AddDays(-10),
            EpochTimePoint(StringToTimestamp("2015-04-25 10:20:34")));
}

TEST(TestEpochTimePoint, TestClearTimeOfDay) {
  EXPECT_EQ(EpochTimePoint(StringToTimestamp("2015-05-05 10:20:34")).ClearTimeOfDay(),
            EpochTimePoint(StringToTimestamp("2015-05-05 00:00:00")));
}

// Tests for precision-aware EpochTimePointT template
TEST(TestEpochTimePointT, TestAllPrecisionsExtractYear) {
  // Test date: 2023-06-15 14:30:45.123456789
  // Unix epoch seconds: 1686839445

  // timestamp[s] - seconds precision
  EpochTimePointSec tp_sec(1686839445LL);
  EXPECT_EQ(tp_sec.TmYear() + 1900, 2023);
  EXPECT_EQ(tp_sec.TmMon() + 1, 6);
  EXPECT_EQ(tp_sec.TmMday(), 15);
  EXPECT_EQ(tp_sec.TmHour(), 14);
  EXPECT_EQ(tp_sec.TmMin(), 30);
  EXPECT_EQ(tp_sec.TmSec(), 45);

  // timestamp[ms] - milliseconds precision
  EpochTimePointMilli tp_ms(1686839445123LL);
  EXPECT_EQ(tp_ms.TmYear() + 1900, 2023);
  EXPECT_EQ(tp_ms.TmMon() + 1, 6);
  EXPECT_EQ(tp_ms.TmMday(), 15);
  EXPECT_EQ(tp_ms.TmHour(), 14);
  EXPECT_EQ(tp_ms.TmMin(), 30);
  EXPECT_EQ(tp_ms.TmSec(), 45);

  // timestamp[us] - microseconds precision
  EpochTimePointMicro tp_us(1686839445123456LL);
  EXPECT_EQ(tp_us.TmYear() + 1900, 2023);
  EXPECT_EQ(tp_us.TmMon() + 1, 6);
  EXPECT_EQ(tp_us.TmMday(), 15);
  EXPECT_EQ(tp_us.TmHour(), 14);
  EXPECT_EQ(tp_us.TmMin(), 30);
  EXPECT_EQ(tp_us.TmSec(), 45);

  // timestamp[ns] - nanoseconds precision
  EpochTimePointNano tp_ns(1686839445123456789LL);
  EXPECT_EQ(tp_ns.TmYear() + 1900, 2023);
  EXPECT_EQ(tp_ns.TmMon() + 1, 6);
  EXPECT_EQ(tp_ns.TmMday(), 15);
  EXPECT_EQ(tp_ns.TmHour(), 14);
  EXPECT_EQ(tp_ns.TmMin(), 30);
  EXPECT_EQ(tp_ns.TmSec(), 45);
}

TEST(TestEpochTimePointT, TestSubSeconds) {
  // timestamp[ms] - subseconds returns milliseconds (0-999)
  EpochTimePointMilli tp_ms(1686839445123LL);
  EXPECT_EQ(tp_ms.SubSeconds(), 123);

  // timestamp[us] - subseconds returns microseconds (0-999999)
  EpochTimePointMicro tp_us(1686839445123456LL);
  EXPECT_EQ(tp_us.SubSeconds(), 123456);

  // timestamp[ns] - subseconds returns nanoseconds (0-999999999)
  EpochTimePointNano tp_ns(1686839445123456789LL);
  EXPECT_EQ(tp_ns.SubSeconds(), 123456789);
}

TEST(TestEpochTimePointT, TestValueSinceEpoch) {
  // Each precision should return the original value
  EpochTimePointSec tp_sec(1686839445LL);
  EXPECT_EQ(tp_sec.ValueSinceEpoch(), 1686839445LL);

  EpochTimePointMilli tp_ms(1686839445123LL);
  EXPECT_EQ(tp_ms.ValueSinceEpoch(), 1686839445123LL);

  EpochTimePointMicro tp_us(1686839445123456LL);
  EXPECT_EQ(tp_us.ValueSinceEpoch(), 1686839445123456LL);

  EpochTimePointNano tp_ns(1686839445123456789LL);
  EXPECT_EQ(tp_ns.ValueSinceEpoch(), 1686839445123456789LL);
}

TEST(TestEpochTimePointT, TestMillisSinceEpoch) {
  // All precisions should convert to milliseconds correctly
  EpochTimePointSec tp_sec(1686839445LL);
  EXPECT_EQ(tp_sec.MillisSinceEpoch(), 1686839445000LL);

  EpochTimePointMilli tp_ms(1686839445123LL);
  EXPECT_EQ(tp_ms.MillisSinceEpoch(), 1686839445123LL);

  EpochTimePointMicro tp_us(1686839445123456LL);
  EXPECT_EQ(tp_us.MillisSinceEpoch(), 1686839445123LL);

  EpochTimePointNano tp_ns(1686839445123456789LL);
  EXPECT_EQ(tp_ns.MillisSinceEpoch(), 1686839445123LL);
}

TEST(TestEpochTimePointT, TestAddDaysPreservesPrecision) {
  // Adding days should preserve sub-day precision
  EpochTimePointMicro tp_us(1686839445123456LL);  // 2023-06-15 14:30:45.123456
  auto tp_us_plus2 = tp_us.AddDays(2);
  EXPECT_EQ(tp_us_plus2.TmMday(), 17);
  // Sub-second portion should be preserved
  EXPECT_EQ(tp_us_plus2.SubSeconds(), 123456);

  EpochTimePointNano tp_ns(1686839445123456789LL);
  auto tp_ns_plus2 = tp_ns.AddDays(2);
  EXPECT_EQ(tp_ns_plus2.TmMday(), 17);
  EXPECT_EQ(tp_ns_plus2.SubSeconds(), 123456789);
}

TEST(TestEpochTimePointT, TestNegativeTimestamps) {
  // Before Unix epoch: 1960-06-15 00:00:00
  // Unix epoch seconds: approximately -301017600
  EpochTimePointSec tp_sec(-301017600LL);
  EXPECT_EQ(tp_sec.TmYear() + 1900, 1960);
  EXPECT_EQ(tp_sec.TmMon() + 1, 6);
  EXPECT_EQ(tp_sec.TmMday(), 15);
}

}  // namespace gandiva
