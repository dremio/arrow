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

// TODO(wesm): IR compilation does not have any include directories set
#include "../../arrow/vendored/datetime/date.h"

bool is_leap_year(int yy);
bool did_days_overflow(arrow_vendored::date::year_month_day ymd);
int last_possible_day_in_month(int month, int year);

// Template class for precision-aware time point operations.
// Duration should be one of: std::chrono::seconds, milliseconds, microseconds, nanoseconds
template <typename Duration>
class EpochTimePointT {
 public:
  using duration_type = Duration;
  using time_point_type =
      std::chrono::time_point<std::chrono::system_clock, Duration>;

  explicit EpochTimePointT(Duration duration_since_epoch)
      : tp_(duration_since_epoch) {}

  explicit EpochTimePointT(int64_t value_since_epoch)
      : EpochTimePointT(Duration(value_since_epoch)) {}

  int TmYear() const { return static_cast<int>(YearMonthDay().year()) - 1900; }

  int TmMon() const { return static_cast<unsigned int>(YearMonthDay().month()) - 1; }

  int TmYday() const {
    auto to_days = arrow_vendored::date::floor<arrow_vendored::date::days>(tp_);
    auto first_day_in_year = arrow_vendored::date::sys_days{
        YearMonthDay().year() / arrow_vendored::date::jan / 1};
    return (to_days - first_day_in_year).count();
  }

  int TmMday() const { return static_cast<unsigned int>(YearMonthDay().day()); }

  int TmWday() const {
    auto to_days = arrow_vendored::date::floor<arrow_vendored::date::days>(tp_);
    return (arrow_vendored::date::weekday{to_days} -  // NOLINT
            arrow_vendored::date::Sunday)
        .count();
  }

  int TmHour() const { return static_cast<int>(TimeOfDay().hours().count()); }

  int TmMin() const { return static_cast<int>(TimeOfDay().minutes().count()); }

  int TmSec() const {
    // TODO(wesm): UNIX y2k issue on int=gdv_int32 platforms
    return static_cast<int>(TimeOfDay().seconds().count());
  }

  // Returns sub-second component in the native duration unit
  // For milliseconds: returns 0-999
  // For microseconds: returns 0-999999
  // For nanoseconds: returns 0-999999999
  int64_t SubSeconds() const {
    auto since_midnight = tp_ - arrow_vendored::date::floor<arrow_vendored::date::days>(tp_);
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(since_midnight);
    return (since_midnight - secs).count();
  }

  EpochTimePointT AddYears(int num_years) const {
    auto ymd = YearMonthDay() + arrow_vendored::date::years(num_years);
    return EpochTimePointT(
        std::chrono::duration_cast<Duration>(
            (arrow_vendored::date::sys_days{ymd} + TimeOfDayDuration()).time_since_epoch()));
  }

  EpochTimePointT AddMonths(int num_months) const {
    auto ymd = YearMonthDay() + arrow_vendored::date::months(num_months);

    EpochTimePointT tp(
        std::chrono::duration_cast<Duration>(
            (arrow_vendored::date::sys_days{ymd} + TimeOfDayDuration()).time_since_epoch()));

    if (did_days_overflow(ymd)) {
      int days_to_offset =
          last_possible_day_in_month(static_cast<int>(ymd.year()),
                                     static_cast<unsigned int>(ymd.month())) -
          static_cast<unsigned int>(ymd.day());
      tp = tp.AddDays(days_to_offset);
    }
    return tp;
  }

  EpochTimePointT AddDays(int num_days) const {
    auto days_since_epoch = arrow_vendored::date::sys_days{YearMonthDay()} +
                            arrow_vendored::date::days(num_days);
    return EpochTimePointT(
        std::chrono::duration_cast<Duration>(
            (days_since_epoch + TimeOfDayDuration()).time_since_epoch()));
  }

  EpochTimePointT ClearTimeOfDay() const {
    return EpochTimePointT(
        std::chrono::duration_cast<Duration>(
            (tp_ - TimeOfDayDuration()).time_since_epoch()));
  }

  bool operator==(const EpochTimePointT& other) const { return tp_ == other.tp_; }

  // Returns the value in the native duration unit
  int64_t ValueSinceEpoch() const { return tp_.time_since_epoch().count(); }

  // For backward compatibility with existing code expecting milliseconds
  int64_t MillisSinceEpoch() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               tp_.time_since_epoch())
        .count();
  }

  arrow_vendored::date::time_of_day<Duration> TimeOfDay() const {
    auto duration_since_midnight =
        tp_ - arrow_vendored::date::floor<arrow_vendored::date::days>(tp_);
    return arrow_vendored::date::time_of_day<Duration>(duration_since_midnight);
  }

 private:
  arrow_vendored::date::year_month_day YearMonthDay() const {
    return arrow_vendored::date::year_month_day{
        arrow_vendored::date::floor<arrow_vendored::date::days>(tp_)};  // NOLINT
  }

  // Returns time of day as a duration for arithmetic operations
  Duration TimeOfDayDuration() const {
    return tp_ - arrow_vendored::date::floor<arrow_vendored::date::days>(tp_);
  }

  time_point_type tp_;
};

// Type aliases for each precision level
using EpochTimePointSec = EpochTimePointT<std::chrono::seconds>;
using EpochTimePointMilli = EpochTimePointT<std::chrono::milliseconds>;
using EpochTimePointMicro = EpochTimePointT<std::chrono::microseconds>;
using EpochTimePointNano = EpochTimePointT<std::chrono::nanoseconds>;

// Backward compatibility: existing code uses EpochTimePoint with milliseconds
using EpochTimePoint = EpochTimePointMilli;
