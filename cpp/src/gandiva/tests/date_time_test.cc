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

#include <gtest/gtest.h>

#include <cmath>
#include <ctime>

#include "arrow/memory_pool.h"
#include "gandiva/precompiled/time_constants.h"
#include "gandiva/projector.h"
#include "gandiva/tests/test_util.h"
#include "gandiva/tree_expr_builder.h"

namespace gandiva {

using arrow::boolean;
using arrow::date32;
using arrow::date64;
using arrow::float32;
using arrow::int32;
using arrow::int64;
using arrow::timestamp;

class DateTimeTestProjector : public ::testing::Test {
 public:
  void SetUp() { pool_ = arrow::default_memory_pool(); }

 protected:
  arrow::MemoryPool* pool_;
};

time_t Epoch() {
  // HACK: MSVC mktime() fails on UTC times before 1970-01-01 00:00:00.
  // But it first converts its argument from local time to UTC time,
  // so we ask for 1970-01-02 to avoid failing in timezones ahead of UTC.
  struct tm y1970;
  memset(&y1970, 0, sizeof(struct tm));
  y1970.tm_year = 70;
  y1970.tm_mon = 0;
  y1970.tm_mday = 2;
  y1970.tm_hour = 0;
  y1970.tm_min = 0;
  y1970.tm_sec = 0;
  time_t epoch = mktime(&y1970);
  if (epoch == static_cast<time_t>(-1)) {
    ARROW_LOG(FATAL) << "mktime() failed";
  }
  // Adjust for the 24h offset above.
  return epoch - 24 * 3600;
}

int32_t MillisInDay(int32_t hh, int32_t mm, int32_t ss, int32_t millis) {
  int32_t mins = hh * 60 + mm;
  int32_t secs = mins * 60 + ss;

  return secs * 1000 + millis;
}

int64_t MillisSince(time_t base_line, int32_t yy, int32_t mm, int32_t dd, int32_t hr,
                    int32_t min, int32_t sec, int32_t millis) {
  struct tm given_ts;
  memset(&given_ts, 0, sizeof(struct tm));
  given_ts.tm_year = (yy - 1900);
  given_ts.tm_mon = (mm - 1);
  given_ts.tm_mday = dd;
  given_ts.tm_hour = hr;
  given_ts.tm_min = min;
  given_ts.tm_sec = sec;

  time_t ts = mktime(&given_ts);
  if (ts == static_cast<time_t>(-1)) {
    ARROW_LOG(FATAL) << "mktime() failed";
  }
  // time_t is an arithmetic type on both POSIX and Windows, we can simply
  // subtract to get a duration in seconds.
  return static_cast<int64_t>(ts - base_line) * 1000 + millis;
}

int32_t DaysSince(time_t base_line, int32_t yy, int32_t mm, int32_t dd, int32_t hr,
                  int32_t min, int32_t sec, int32_t millis) {
  struct tm given_ts;
  memset(&given_ts, 0, sizeof(struct tm));
  given_ts.tm_year = (yy - 1900);
  given_ts.tm_mon = (mm - 1);
  given_ts.tm_mday = dd;
  given_ts.tm_hour = hr;
  given_ts.tm_min = min;
  given_ts.tm_sec = sec;

  time_t ts = mktime(&given_ts);
  if (ts == static_cast<time_t>(-1)) {
    ARROW_LOG(FATAL) << "mktime() failed";
  }
  // time_t is an arithmetic type on both POSIX and Windows, we can simply
  // subtract to get a duration in seconds.
  return static_cast<int32_t>(((ts - base_line) * 1000 + millis) / MILLIS_IN_DAY);
}

TEST_F(DateTimeTestProjector, TestIsNull) {
  auto d0 = field("d0", date64());
  auto t0 = field("t0", time32(arrow::TimeUnit::MILLI));
  auto schema = arrow::schema({d0, t0});

  // output fields
  auto b0 = field("isnull", boolean());

  // isnull and isnotnull
  auto isnull_expr = TreeExprBuilder::MakeExpression("isnull", {d0}, b0);
  auto isnotnull_expr = TreeExprBuilder::MakeExpression("isnotnull", {t0}, b0);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {isnull_expr, isnotnull_expr},
                                TestConfiguration(), &projector);
  ASSERT_TRUE(status.ok());

  int num_records = 4;
  std::vector<int64_t> d0_data = {0, 100, 0, 1000};
  auto t0_data = {0, 100, 0, 1000};
  auto validity = {false, true, false, true};
  auto d0_array =
      MakeArrowTypeArray<arrow::Date64Type, int64_t>(date64(), d0_data, validity);
  auto t0_array = MakeArrowTypeArray<arrow::Time32Type, int32_t>(
      time32(arrow::TimeUnit::MILLI), t0_data, validity);

  // expected output
  auto exp_isnull =
      MakeArrowArrayBool({true, false, true, false}, {true, true, true, true});
  auto exp_isnotnull = MakeArrowArrayBool(validity, {true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {d0_array, t0_array});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_isnull, outputs.at(0));
  EXPECT_ARROW_ARRAY_EQUALS(exp_isnotnull, outputs.at(1));
}

TEST_F(DateTimeTestProjector, TestDate32IsNull) {
  auto d0 = field("d0", date32());
  auto schema = arrow::schema({d0});

  // output fields
  auto b0 = field("isnull", boolean());

  // isnull and isnotnull
  auto isnull_expr = TreeExprBuilder::MakeExpression("isnull", {d0}, b0);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {isnull_expr}, TestConfiguration(), &projector);
  ASSERT_TRUE(status.ok());

  int num_records = 4;
  std::vector<int32_t> d0_data = {0, 100, 0, 1000};
  auto validity = {false, true, false, true};
  auto d0_array =
      MakeArrowTypeArray<arrow::Date32Type, int32_t>(date32(), d0_data, validity);

  // expected output
  auto exp_isnull =
      MakeArrowArrayBool({true, false, true, false}, {true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {d0_array});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_isnull, outputs.at(0));
}

TEST_F(DateTimeTestProjector, TestDateTime) {
  auto field0 = field("f0", date64());
  auto field1 = field("f1", date32());
  auto field2 = field("f2", timestamp(arrow::TimeUnit::MILLI));
  auto schema = arrow::schema({field0, field1, field2});

  // output fields
  auto field_year = field("yy", int64());
  auto field_month = field("mm", int64());
  auto field_day = field("dd", int64());
  auto field_hour = field("hh", int64());
  auto field_date64 = field("date64", date64());

  // extract year and month from date
  auto date2year_expr =
      TreeExprBuilder::MakeExpression("extractYear", {field0}, field_year);
  auto date2month_expr =
      TreeExprBuilder::MakeExpression("extractMonth", {field0}, field_month);

  // extract year and month from date32, cast to date64 first
  auto node_f1 = TreeExprBuilder::MakeField(field1);
  auto date32_to_date64_func =
      TreeExprBuilder::MakeFunction("castDATE", {node_f1}, date64());

  auto date64_2year_func =
      TreeExprBuilder::MakeFunction("extractYear", {date32_to_date64_func}, int64());
  auto date64_2year_expr = TreeExprBuilder::MakeExpression(date64_2year_func, field_year);

  auto date64_2month_func =
      TreeExprBuilder::MakeFunction("extractMonth", {date32_to_date64_func}, int64());
  auto date64_2month_expr =
      TreeExprBuilder::MakeExpression(date64_2month_func, field_month);

  // extract month and day from timestamp
  auto ts2month_expr =
      TreeExprBuilder::MakeExpression("extractMonth", {field2}, field_month);
  auto ts2day_expr = TreeExprBuilder::MakeExpression("extractDay", {field2}, field_day);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema,
                                {date2year_expr, date2month_expr, date64_2year_expr,
                                 date64_2month_expr, ts2month_expr, ts2day_expr},
                                TestConfiguration(), &projector);
  ASSERT_TRUE(status.ok());

  // Create a row-batch with some sample data
  time_t epoch = Epoch();
  int num_records = 4;
  auto validity = {true, true, true, true};
  std::vector<int64_t> field0_data = {MillisSince(epoch, 2000, 1, 1, 5, 0, 0, 0),
                                      MillisSince(epoch, 1999, 12, 31, 5, 0, 0, 0),
                                      MillisSince(epoch, 2015, 6, 30, 20, 0, 0, 0),
                                      MillisSince(epoch, 2015, 7, 1, 20, 0, 0, 0)};
  auto array0 =
      MakeArrowTypeArray<arrow::Date64Type, int64_t>(date64(), field0_data, validity);

  std::vector<int32_t> field1_data = {DaysSince(epoch, 2000, 1, 1, 5, 0, 0, 0),
                                      DaysSince(epoch, 1999, 12, 31, 5, 0, 0, 0),
                                      DaysSince(epoch, 2015, 6, 30, 20, 0, 0, 0),
                                      DaysSince(epoch, 2015, 7, 1, 20, 0, 0, 0)};
  auto array1 =
      MakeArrowTypeArray<arrow::Date32Type, int32_t>(date32(), field1_data, validity);

  std::vector<int64_t> field2_data = {MillisSince(epoch, 1999, 12, 31, 5, 0, 0, 0),
                                      MillisSince(epoch, 2000, 1, 2, 5, 0, 0, 0),
                                      MillisSince(epoch, 2015, 7, 1, 1, 0, 0, 0),
                                      MillisSince(epoch, 2015, 6, 29, 23, 0, 0, 0)};

  auto array2 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), field2_data, validity);

  // expected output
  // date 2 year and date 2 month for date64
  auto exp_yy_from_date64 = MakeArrowArrayInt64({2000, 1999, 2015, 2015}, validity);
  auto exp_mm_from_date64 = MakeArrowArrayInt64({1, 12, 6, 7}, validity);

  // date 2 year and date 2 month for date32
  auto exp_yy_from_date32 = MakeArrowArrayInt64({2000, 1999, 2015, 2015}, validity);
  auto exp_mm_from_date32 = MakeArrowArrayInt64({1, 12, 6, 7}, validity);

  // ts 2 month and ts 2 day
  auto exp_mm_from_ts = MakeArrowArrayInt64({12, 1, 7, 6}, validity);
  auto exp_dd_from_ts = MakeArrowArrayInt64({31, 2, 1, 29}, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1, array2});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_yy_from_date64, outputs.at(0));
  EXPECT_ARROW_ARRAY_EQUALS(exp_mm_from_date64, outputs.at(1));
  EXPECT_ARROW_ARRAY_EQUALS(exp_yy_from_date32, outputs.at(2));
  EXPECT_ARROW_ARRAY_EQUALS(exp_mm_from_date32, outputs.at(3));
  EXPECT_ARROW_ARRAY_EQUALS(exp_mm_from_ts, outputs.at(4));
  EXPECT_ARROW_ARRAY_EQUALS(exp_dd_from_ts, outputs.at(5));
}

TEST_F(DateTimeTestProjector, TestTime) {
  auto field0 = field("f0", time32(arrow::TimeUnit::MILLI));
  auto schema = arrow::schema({field0});

  auto field_min = field("mm", int64());
  auto field_hour = field("hh", int64());

  // extract day and hour from time32
  auto time2min_expr =
      TreeExprBuilder::MakeExpression("extractMinute", {field0}, field_min);
  auto time2hour_expr =
      TreeExprBuilder::MakeExpression("extractHour", {field0}, field_hour);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {time2min_expr, time2hour_expr},
                                TestConfiguration(), &projector);
  ASSERT_TRUE(status.ok());

  // create input data
  int num_records = 4;
  auto validity = {true, true, true, true};
  std::vector<int32_t> field_data = {
      MillisInDay(5, 35, 25, 0),  // 5:35:25
      MillisInDay(0, 59, 0, 0),   // 0:59:12
      MillisInDay(12, 30, 0, 0),  // 12:30:0
      MillisInDay(23, 0, 0, 0)    // 23:0:0
  };
  auto array = MakeArrowTypeArray<arrow::Time32Type, int32_t>(
      time32(arrow::TimeUnit::MILLI), field_data, validity);

  // expected output
  auto exp_min = MakeArrowArrayInt64({35, 59, 30, 0}, validity);
  auto exp_hour = MakeArrowArrayInt64({5, 0, 12, 23}, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_min, outputs.at(0));
  EXPECT_ARROW_ARRAY_EQUALS(exp_hour, outputs.at(1));
}

TEST_F(DateTimeTestProjector, TestTimestampDiff) {
  auto f0 = field("f0", timestamp(arrow::TimeUnit::MILLI));
  auto f1 = field("f1", timestamp(arrow::TimeUnit::MILLI));
  auto schema = arrow::schema({f0, f1});

  // output fields
  auto diff_seconds = field("ss", int32());

  // get diff
  auto diff_secs_expr =
      TreeExprBuilder::MakeExpression("timestampdiffSecond", {f0, f1}, diff_seconds);

  auto diff_mins_expr =
      TreeExprBuilder::MakeExpression("timestampdiffMinute", {f0, f1}, diff_seconds);

  auto diff_hours_expr =
      TreeExprBuilder::MakeExpression("timestampdiffHour", {f0, f1}, diff_seconds);

  auto diff_days_expr =
      TreeExprBuilder::MakeExpression("timestampdiffDay", {f0, f1}, diff_seconds);

  auto diff_days_expr_with_datediff_fn =
      TreeExprBuilder::MakeExpression("datediff", {f0, f1}, diff_seconds);

  auto diff_weeks_expr =
      TreeExprBuilder::MakeExpression("timestampdiffWeek", {f0, f1}, diff_seconds);

  auto diff_months_expr =
      TreeExprBuilder::MakeExpression("timestampdiffMonth", {f0, f1}, diff_seconds);

  auto diff_quarters_expr =
      TreeExprBuilder::MakeExpression("timestampdiffQuarter", {f0, f1}, diff_seconds);

  auto diff_years_expr =
      TreeExprBuilder::MakeExpression("timestampdiffYear", {f0, f1}, diff_seconds);

  std::shared_ptr<Projector> projector;
  auto exprs = {diff_secs_expr,
                diff_mins_expr,
                diff_hours_expr,
                diff_days_expr,
                diff_days_expr_with_datediff_fn,
                diff_weeks_expr,
                diff_months_expr,
                diff_quarters_expr,
                diff_years_expr};
  auto status = Projector::Make(schema, exprs, TestConfiguration(), &projector);
  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  // 2015-09-10T20:49:42.000
  auto start_millis = MillisSince(epoch, 2015, 9, 10, 20, 49, 42, 0);
  // 2017-03-30T22:50:59.050
  auto end_millis = MillisSince(epoch, 2017, 3, 30, 22, 50, 59, 50);
  std::vector<int64_t> f0_data = {start_millis, end_millis,
                                  // 2015-09-10T20:49:42.999
                                  start_millis + 999,
                                  // 2015-09-10T20:49:42.999
                                  MillisSince(epoch, 2015, 9, 10, 20, 49, 42, 999)};
  std::vector<int64_t> f1_data = {end_millis, start_millis,
                                  // 2015-09-10T20:49:42.999
                                  start_millis + 999,
                                  // 2015-09-9T21:49:42.999 (23 hours behind)
                                  MillisSince(epoch, 2015, 9, 9, 21, 49, 42, 999)};

  int64_t num_records = f0_data.size();
  std::vector<bool> validity(num_records, true);
  auto array0 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), f0_data, validity);
  auto array1 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), f1_data, validity);

  // expected output
  std::vector<ArrayPtr> exp_output;
  exp_output.push_back(
      MakeArrowArrayInt32({48996077, -48996077, 0, -23 * 3600}, validity));
  exp_output.push_back(MakeArrowArrayInt32({816601, -816601, 0, -23 * 60}, validity));
  exp_output.push_back(MakeArrowArrayInt32({13610, -13610, 0, -23}, validity));
  exp_output.push_back(MakeArrowArrayInt32({567, -567, 0, 0}, validity));
  exp_output.push_back(MakeArrowArrayInt32({-567, 567, 0, 0}, validity));
  exp_output.push_back(MakeArrowArrayInt32({81, -81, 0, 0}, validity));
  exp_output.push_back(MakeArrowArrayInt32({18, -18, 0, 0}, validity));
  exp_output.push_back(MakeArrowArrayInt32({6, -6, 0, 0}, validity));
  exp_output.push_back(MakeArrowArrayInt32({1, -1, 0, 0}, validity));

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  for (uint32_t i = 0; i < exp_output.size(); i++) {
    EXPECT_ARROW_ARRAY_EQUALS(exp_output.at(i), outputs.at(i));
  }
}

TEST_F(DateTimeTestProjector, TestTimestampDiffMonth) {
  auto f0 = field("f0", timestamp(arrow::TimeUnit::MILLI));
  auto f1 = field("f1", timestamp(arrow::TimeUnit::MILLI));
  auto schema = arrow::schema({f0, f1});

  // output fields
  auto diff_seconds = field("ss", int32());

  auto diff_months_expr =
      TreeExprBuilder::MakeExpression("timestampdiffMonth", {f0, f1}, diff_seconds);

  std::shared_ptr<Projector> projector;
  auto status =
      Projector::Make(schema, {diff_months_expr}, TestConfiguration(), &projector);

  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  // Create a row-batch with some sample data
  std::vector<int64_t> f0_data = {MillisSince(epoch, 2019, 1, 31, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 1, 31, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 1, 31, 0, 0, 0, 0),
                                  MillisSince(epoch, 2019, 3, 31, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 3, 30, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 5, 31, 0, 0, 0, 0)};
  std::vector<int64_t> f1_data = {MillisSince(epoch, 2019, 2, 28, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 2, 28, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 2, 29, 0, 0, 0, 0),
                                  MillisSince(epoch, 2019, 4, 30, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 2, 29, 0, 0, 0, 0),
                                  MillisSince(epoch, 2020, 9, 30, 0, 0, 0, 0)};
  int64_t num_records = f0_data.size();
  std::vector<bool> validity(num_records, true);

  auto array0 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), f0_data, validity);
  auto array1 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), f1_data, validity);

  // expected output
  std::vector<ArrayPtr> exp_output;
  exp_output.push_back(MakeArrowArrayInt32({1, 0, 1, 1, -1, 4}, validity));

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  for (uint32_t i = 0; i < exp_output.size(); i++) {
    EXPECT_ARROW_ARRAY_EQUALS(exp_output.at(i), outputs.at(i));
  }
}

TEST_F(DateTimeTestProjector, TestMonthsBetween) {
  auto f0 = field("f0", arrow::date64());
  auto f1 = field("f1", arrow::date64());
  auto schema = arrow::schema({f0, f1});

  // output fields
  auto output = field("out", arrow::float64());

  auto months_between_expr =
      TreeExprBuilder::MakeExpression("months_between", {f0, f1}, output);

  std::shared_ptr<Projector> projector;
  auto status =
      Projector::Make(schema, {months_between_expr}, TestConfiguration(), &projector);

  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  // Create a row-batch with some sample data
  int num_records = 4;
  auto validity = {true, true, true, true};
  std::vector<int64_t> f0_data = {MillisSince(epoch, 1995, 3, 2, 0, 0, 0, 0),
                                  MillisSince(epoch, 1995, 2, 2, 0, 0, 0, 0),
                                  MillisSince(epoch, 1995, 3, 31, 0, 0, 0, 0),
                                  MillisSince(epoch, 1996, 3, 31, 0, 0, 0, 0)};

  auto array0 =
      MakeArrowTypeArray<arrow::Date64Type, int64_t>(date64(), f0_data, validity);

  std::vector<int64_t> f1_data = {MillisSince(epoch, 1995, 2, 2, 0, 0, 0, 0),
                                  MillisSince(epoch, 1995, 3, 2, 0, 0, 0, 0),
                                  MillisSince(epoch, 1995, 2, 28, 0, 0, 0, 0),
                                  MillisSince(epoch, 1996, 2, 29, 0, 0, 0, 0)};

  auto array1 =
      MakeArrowTypeArray<arrow::Date64Type, int64_t>(date64(), f1_data, validity);

  // expected output
  auto exp_output = MakeArrowArrayFloat64({1.0, -1.0, 1.0, 1.0}, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(0));
}

TEST_F(DateTimeTestProjector, TestCastTimestampFromInt64) {
  auto f0 = field("f0", arrow::int64());
  auto schema = arrow::schema({f0});

  // output fields
  auto output = field("out", arrow::timestamp(arrow::TimeUnit::MILLI));

  auto casttimestamp_expr =
      TreeExprBuilder::MakeExpression("castTIMESTAMP", {f0}, output);

  std::shared_ptr<Projector> projector;
  auto status =
      Projector::Make(schema, {casttimestamp_expr}, TestConfiguration(), &projector);
  std::cout << status.message();
  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  int num_records = 5;
  auto validity = {true, true, true, true, true};
  std::vector<int64_t> f0_data = {MillisSince(epoch, 2016, 2, 3, 8, 20, 10, 34),
                                  MillisSince(epoch, 2016, 2, 29, 23, 59, 59, 59),
                                  MillisSince(epoch, 2016, 1, 30, 1, 15, 20, 0),
                                  MillisSince(epoch, 2017, 2, 3, 23, 15, 20, 0),
                                  MillisSince(epoch, 1970, 12, 30, 22, 50, 11, 0)};

  auto array0 = MakeArrowArrayInt64(f0_data, validity);

  std::vector<int64_t> f0_output_data = {MillisSince(epoch, 2016, 2, 3, 8, 20, 10, 34),
                                         MillisSince(epoch, 2016, 2, 29, 23, 59, 59, 59),
                                         MillisSince(epoch, 2016, 1, 30, 1, 15, 20, 0),
                                         MillisSince(epoch, 2017, 2, 3, 23, 15, 20, 0),
                                         MillisSince(epoch, 1970, 12, 30, 22, 50, 11, 0)};

  // expected output
  auto exp_output = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      timestamp(arrow::TimeUnit::MILLI), f0_output_data, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(0));
}

TEST_F(DateTimeTestProjector, TestLastDay) {
  auto f0 = field("f0", arrow::date64());
  auto schema = arrow::schema({f0});

  // output fields
  auto output = field("out", arrow::date64());

  auto last_day_expr = TreeExprBuilder::MakeExpression("last_day", {f0}, output);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {last_day_expr}, TestConfiguration(), &projector);

  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  // Create a row-batch with some sample data
  // Used a leap year as example.
  int num_records = 5;
  auto validity = {true, true, true, true, true};
  std::vector<int64_t> f0_data = {MillisSince(epoch, 2016, 2, 3, 8, 20, 10, 34),
                                  MillisSince(epoch, 2016, 2, 29, 23, 59, 59, 59),
                                  MillisSince(epoch, 2016, 1, 30, 1, 15, 20, 0),
                                  MillisSince(epoch, 2017, 2, 3, 23, 15, 20, 0),
                                  MillisSince(epoch, 2015, 12, 30, 22, 50, 11, 0)};

  auto array0 =
      MakeArrowTypeArray<arrow::Date64Type, int64_t>(date64(), f0_data, validity);

  std::vector<int64_t> f0_output_data = {MillisSince(epoch, 2016, 2, 29, 0, 0, 0, 0),
                                         MillisSince(epoch, 2016, 2, 29, 0, 0, 0, 0),
                                         MillisSince(epoch, 2016, 1, 31, 0, 0, 0, 0),
                                         MillisSince(epoch, 2017, 2, 28, 0, 0, 0, 0),
                                         MillisSince(epoch, 2015, 12, 31, 0, 0, 0, 0)};

  // expected output
  auto exp_output = MakeArrowArrayDate64(f0_output_data, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(0));
}

TEST_F(DateTimeTestProjector, TestToTimestampFromInt) {
  auto f0 = field("f0", arrow::int32());
  auto f1 = field("f1", arrow::int64());
  auto f2 = field("f2", arrow::float32());
  auto f3 = field("f3", arrow::float64());
  auto schema = arrow::schema({f0, f1, f2, f3});

  // output fields
  auto output = field("out", arrow::timestamp(arrow::TimeUnit::MILLI));
  auto output1 = field("out1", arrow::timestamp(arrow::TimeUnit::MILLI));
  auto output2 = field("out1", arrow::timestamp(arrow::TimeUnit::MILLI));
  auto output3 = field("out1", arrow::timestamp(arrow::TimeUnit::MILLI));

  auto totimestamp_expr = TreeExprBuilder::MakeExpression("to_timestamp", {f0}, output);
  auto totimestamp_expr1 = TreeExprBuilder::MakeExpression("to_timestamp", {f1}, output1);
  auto totimestamp_expr2 = TreeExprBuilder::MakeExpression("to_timestamp", {f2}, output2);
  auto totimestamp_expr3 = TreeExprBuilder::MakeExpression("to_timestamp", {f3}, output3);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(
      schema, {totimestamp_expr, totimestamp_expr1, totimestamp_expr2, totimestamp_expr3},
      TestConfiguration(), &projector);
  std::cout << status.message();
  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  int num_records = 3;
  auto validity = {true, true, false};
  std::vector<int32_t> f0_data = {0, 1626255099, 0};
  std::vector<int64_t> f1_data = {0, 1626255099, 0};
  std::vector<float> f2_data = {0, 3601.411f, 0};
  std::vector<double> f3_data = {0, 3601.411, 0};

  auto array0 = MakeArrowArrayInt32(f0_data, validity);
  auto array1 = MakeArrowArrayInt64(f1_data, validity);
  auto array2 = MakeArrowArrayFloat32(f2_data, validity);
  auto array3 = MakeArrowArrayFloat64(f3_data, validity);

  std::vector<int64_t> f0_1_output_data = {MillisSince(epoch, 1970, 1, 1, 0, 0, 0, 0),
                                           MillisSince(epoch, 2021, 7, 14, 9, 31, 39, 0),
                                           0};

  std::vector<int64_t> f2_3_output_data = {MillisSince(epoch, 1970, 1, 1, 0, 0, 0, 0),
                                           MillisSince(epoch, 1970, 1, 1, 1, 0, 1, 411),
                                           0};

  // expected output
  auto exp_output = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      timestamp(arrow::TimeUnit::MILLI), f0_1_output_data, validity);

  // expected output
  auto exp_output1 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      timestamp(arrow::TimeUnit::MILLI), f2_3_output_data, validity);

  // prepare input record batch
  auto in_batch =
      arrow::RecordBatch::Make(schema, num_records, {array0, array1, array2, array3});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(0));
  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(1));
  EXPECT_ARROW_ARRAY_EQUALS(exp_output1, outputs.at(2));
  EXPECT_ARROW_ARRAY_EQUALS(exp_output1, outputs.at(3));
}

TEST_F(DateTimeTestProjector, TestToUtcTimestamp) {
  auto f0 = field("f0", timestamp(arrow::TimeUnit::MILLI));
  auto f1 = field("f1", arrow::utf8());

  auto schema = arrow::schema({f0, f1});

  // output fields
  auto utc_timestamp = field("utc_time", timestamp(arrow::TimeUnit::MILLI));

  auto utc_time_expr =
      TreeExprBuilder::MakeExpression("to_utc_timestamp", {f0, f1}, utc_timestamp);
  std::shared_ptr<Projector> projector;
  Status status =
      Projector::Make(schema, {utc_time_expr}, TestConfiguration(), &projector);

  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  // Create a row-batch with some sample data
  std::vector<int64_t> f0_data = {MillisSince(epoch, 1970, 1, 1, 6, 0, 0, 0),
                                  MillisSince(epoch, 2001, 1, 5, 3, 0, 0, 0),
                                  MillisSince(epoch, 2018, 3, 12, 1, 0, 0, 0),
                                  MillisSince(epoch, 2018, 3, 11, 1, 0, 0, 0)};
  int64_t num_records = f0_data.size();
  std::vector<bool> validity(num_records, true);
  auto array0 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), f0_data, validity);

  auto array1 = MakeArrowArrayUtf8(
      {"Asia/Kolkata", "Asia/Kolkata", "America/Los_Angeles", "America/Los_Angeles"},
      {true, true, true, true});

  // expected output
  std::vector<int64_t> exp_output_data = {MillisSince(epoch, 1970, 1, 1, 0, 30, 0, 0),
                                          MillisSince(epoch, 2001, 1, 4, 21, 30, 0, 0),
                                          MillisSince(epoch, 2018, 3, 12, 8, 0, 0, 0),
                                          MillisSince(epoch, 2018, 3, 11, 9, 0, 0, 0)};
  auto exp_output = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), exp_output_data, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results

  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(0));
}

TEST_F(DateTimeTestProjector, TestFromUtcTimestamp) {
  auto f0 = field("f0", timestamp(arrow::TimeUnit::MILLI));
  auto f1 = field("f1", arrow::utf8());

  auto schema = arrow::schema({f0, f1});

  // output fields
  auto local_timestamp = field("local_time", timestamp(arrow::TimeUnit::MILLI));

  auto local_time_expr =
      TreeExprBuilder::MakeExpression("from_utc_timestamp", {f0, f1}, local_timestamp);
  std::shared_ptr<Projector> projector;
  Status status =
      Projector::Make(schema, {local_time_expr}, TestConfiguration(), &projector);

  ASSERT_TRUE(status.ok());

  time_t epoch = Epoch();

  // Create a row-batch with some sample data
  std::vector<int64_t> f0_data = {MillisSince(epoch, 1970, 1, 1, 0, 30, 0, 0),
                                  MillisSince(epoch, 2001, 1, 4, 21, 30, 0, 0),
                                  MillisSince(epoch, 2018, 3, 12, 8, 0, 0, 0),
                                  MillisSince(epoch, 2018, 3, 11, 9, 0, 0, 0)};

  int64_t num_records = f0_data.size();
  std::vector<bool> validity(num_records, true);
  auto array0 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), f0_data, validity);

  auto array1 = MakeArrowArrayUtf8(
      {"Asia/Kolkata", "Asia/Kolkata", "America/Los_Angeles", "America/Los_Angeles"},
      {true, true, true, true});

  // expected output
  std::vector<int64_t> exp_output_data = {MillisSince(epoch, 1970, 1, 1, 6, 0, 0, 0),
                                          MillisSince(epoch, 2001, 1, 5, 3, 0, 0, 0),
                                          MillisSince(epoch, 2018, 3, 12, 1, 0, 0, 0),
                                          MillisSince(epoch, 2018, 3, 11, 1, 0, 0, 0)};
  auto exp_output = MakeArrowTypeArray<arrow::TimestampType, int64_t>(
      arrow::timestamp(arrow::TimeUnit::MILLI), exp_output_data, validity);

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp_output, outputs.at(0));
}

// ---------- Timestamp precision tests (ms, us, ns) ----------

// 2021-06-15T14:30:45.123Z in millis since epoch
static const int64_t kTestMillis = 1623767445123LL;
static const int64_t kSubMs = 456;  // sub-millisecond micros
static const int64_t kSubUs = 789;  // sub-microsecond nanos
static const int64_t kTestMicros = kTestMillis * 1000 + kSubMs;
static const int64_t kTestNanos = kTestMillis * 1000000 + kSubMs * 1000 + kSubUs;

// Helper: evaluate a unary timestamp function returning int64
static int64_t EvalExtract(const std::string& func_name, arrow::TimeUnit::type unit,
                           int64_t ts_value, arrow::MemoryPool* pool) {
  auto ts_type = timestamp(unit);
  auto f0 = field("f0", ts_type);
  auto schema = arrow::schema({f0});
  auto result_field = field("result", int64());
  auto expr = TreeExprBuilder::MakeExpression(func_name, {f0}, result_field);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  auto in_array =
      MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
  auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok());

  auto result_array = std::dynamic_pointer_cast<arrow::Int64Array>(outputs.at(0));
  return result_array->Value(0);
}

// Helper: evaluate a unary timestamp function returning timestamp
static int64_t EvalTrunc(const std::string& func_name, arrow::TimeUnit::type unit,
                         int64_t ts_value, arrow::MemoryPool* pool) {
  auto ts_type = timestamp(unit);
  auto f0 = field("f0", ts_type);
  auto schema = arrow::schema({f0});
  auto result_field = field("result", ts_type);
  auto expr = TreeExprBuilder::MakeExpression(func_name, {f0}, result_field);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  auto in_array =
      MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
  auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok());

  // Result is a TimestampArray, not Int64Array — use raw values buffer.
  auto result_array = std::dynamic_pointer_cast<arrow::TimestampArray>(outputs.at(0));
  return result_array->Value(0);
}

TEST_F(DateTimeTestProjector, TestExtractAcrossPrecisions) {
  // extractMonth must return 6 for all precisions
  EXPECT_EQ(6, EvalExtract("extractMonth", arrow::TimeUnit::MILLI, kTestMillis, pool_));
  EXPECT_EQ(6, EvalExtract("extractMonth", arrow::TimeUnit::MICRO, kTestMicros, pool_));
  EXPECT_EQ(6, EvalExtract("extractMonth", arrow::TimeUnit::NANO, kTestNanos, pool_));

  // extractDay must return 15
  EXPECT_EQ(15, EvalExtract("extractDay", arrow::TimeUnit::MILLI, kTestMillis, pool_));
  EXPECT_EQ(15, EvalExtract("extractDay", arrow::TimeUnit::MICRO, kTestMicros, pool_));
  EXPECT_EQ(15, EvalExtract("extractDay", arrow::TimeUnit::NANO, kTestNanos, pool_));

  // extractHour must return 14
  EXPECT_EQ(14, EvalExtract("extractHour", arrow::TimeUnit::MILLI, kTestMillis, pool_));
  EXPECT_EQ(14, EvalExtract("extractHour", arrow::TimeUnit::MICRO, kTestMicros, pool_));
  EXPECT_EQ(14, EvalExtract("extractHour", arrow::TimeUnit::NANO, kTestNanos, pool_));

  // extractYear must return 2021
  EXPECT_EQ(2021, EvalExtract("extractYear", arrow::TimeUnit::MILLI, kTestMillis, pool_));
  EXPECT_EQ(2021, EvalExtract("extractYear", arrow::TimeUnit::MICRO, kTestMicros, pool_));
  EXPECT_EQ(2021, EvalExtract("extractYear", arrow::TimeUnit::NANO, kTestNanos, pool_));
}

TEST_F(DateTimeTestProjector, TestDateTruncAcrossPrecisions) {
  // 2021-06-15T00:00:00Z in millis
  int64_t day_millis = 1623715200000LL;

  // date_trunc_Day: millis
  EXPECT_EQ(day_millis,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::MILLI, kTestMillis, pool_));
  // date_trunc_Day: micros — sub-ms data zeroed
  EXPECT_EQ(day_millis * 1000,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::MICRO, kTestMicros, pool_));
  // date_trunc_Day: nanos — sub-ms data zeroed
  EXPECT_EQ(day_millis * 1000000,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::NANO, kTestNanos, pool_));

  // 2021-06-15T14:00:00Z in millis
  int64_t hour_millis = 1623765600000LL;
  EXPECT_EQ(hour_millis * 1000,
            EvalTrunc("date_trunc_Hour", arrow::TimeUnit::MICRO, kTestMicros, pool_));
  EXPECT_EQ(hour_millis * 1000000,
            EvalTrunc("date_trunc_Hour", arrow::TimeUnit::NANO, kTestNanos, pool_));
}

// Helper: evaluate timestampadd(int32, timestamp) -> timestamp
static int64_t EvalTimestampadd(const std::string& func_name, arrow::TimeUnit::type unit,
                                int32_t count, int64_t ts_value,
                                arrow::MemoryPool* pool) {
  auto ts_type = timestamp(unit);
  auto f_count = field("count", int32());
  auto f_ts = field("ts", ts_type);
  auto schema = arrow::schema({f_count, f_ts});
  auto result_field = field("result", ts_type);

  auto count_node = TreeExprBuilder::MakeField(f_count);
  auto ts_node = TreeExprBuilder::MakeField(f_ts);
  auto func_node =
      TreeExprBuilder::MakeFunction(func_name, {count_node, ts_node}, ts_type);
  auto expr = TreeExprBuilder::MakeExpression(func_node, result_field);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  auto count_array =
      MakeArrowTypeArray<arrow::Int32Type, int32_t>(int32(), {count}, {true});
  auto ts_array =
      MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
  auto in_batch = arrow::RecordBatch::Make(schema, 1, {count_array, ts_array});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok());

  auto result_array = std::dynamic_pointer_cast<arrow::TimestampArray>(outputs.at(0));
  return result_array->Value(0);
}

TEST_F(DateTimeTestProjector, TestTimestampaddSecondPreservesSubMs) {
  // Add 10 seconds to a timestamp with sub-ms precision.
  // Sub-ms data must survive.

  // Micros: 10 seconds = 10_000_000 us. Sub-ms 456 preserved.
  int64_t r = EvalTimestampadd("timestampaddSecond", arrow::TimeUnit::MICRO, 10,
                               kTestMicros, pool_);
  EXPECT_EQ(kTestMicros + 10LL * 1000000, r);
  EXPECT_EQ(kSubMs, r % 1000);

  // Nanos: 10 seconds = 10_000_000_000 ns. Sub-us 789 preserved.
  r = EvalTimestampadd("timestampaddSecond", arrow::TimeUnit::NANO, 10, kTestNanos,
                       pool_);
  EXPECT_EQ(kTestNanos + 10LL * 1000000000, r);
  EXPECT_EQ(kSubUs, r % 1000);
}

TEST_F(DateTimeTestProjector, TestTimestampaddDayPreservesSubMs) {
  // Add 1 day. Sub-ms must survive.
  int64_t r =
      EvalTimestampadd("timestampaddDay", arrow::TimeUnit::MICRO, 1, kTestMicros, pool_);
  EXPECT_EQ(kTestMicros + 86400LL * 1000000, r);
  EXPECT_EQ(kSubMs, r % 1000);

  r = EvalTimestampadd("timestampaddDay", arrow::TimeUnit::NANO, 1, kTestNanos, pool_);
  EXPECT_EQ(kTestNanos + 86400LL * 1000000000, r);
  EXPECT_EQ(kSubUs, r % 1000);
}

TEST_F(DateTimeTestProjector, TestTimestampaddMonthPreservesSubMs) {
  // Add 2 months (calendar math). Sub-ms data must survive.
  // Use millis result as ground truth — micros/nanos must match with sub-ms appended.
  int64_t base_millis = EvalTimestampadd("timestampaddMonth", arrow::TimeUnit::MILLI, 2,
                                         kTestMillis, pool_);

  int64_t r = EvalTimestampadd("timestampaddMonth", arrow::TimeUnit::MICRO, 2,
                               kTestMicros, pool_);
  EXPECT_EQ(base_millis * 1000 + kSubMs, r);

  r = EvalTimestampadd("timestampaddMonth", arrow::TimeUnit::NANO, 2, kTestNanos, pool_);
  EXPECT_EQ(base_millis * 1000000 + kSubMs * 1000 + kSubUs, r);
}

// Helper: evaluate a two-timestamp function returning int32
static int32_t EvalDiff(const std::string& func_name, arrow::TimeUnit::type unit,
                        int64_t ts1, int64_t ts2, arrow::MemoryPool* pool) {
  auto ts_type = timestamp(unit);
  auto f1 = field("f1", ts_type);
  auto f2 = field("f2", ts_type);
  auto schema = arrow::schema({f1, f2});
  auto result_field = field("result", int32());

  auto n1 = TreeExprBuilder::MakeField(f1);
  auto n2 = TreeExprBuilder::MakeField(f2);
  auto func_node = TreeExprBuilder::MakeFunction(func_name, {n1, n2}, int32());
  auto expr = TreeExprBuilder::MakeExpression(func_node, result_field);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  auto a1 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts1}, {true});
  auto a2 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts2}, {true});
  auto in_batch = arrow::RecordBatch::Make(schema, 1, {a1, a2});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok());

  auto result_array = std::dynamic_pointer_cast<arrow::Int32Array>(outputs.at(0));
  return result_array->Value(0);
}

TEST_F(DateTimeTestProjector, TestTimestampdiffAcrossPrecisions) {
  // timestampdiffDay between kTestMillis and kTestMillis + 3 days
  int64_t three_days_later_ms = kTestMillis + 3 * 86400000LL;
  EXPECT_EQ(3, EvalDiff("timestampdiffDay", arrow::TimeUnit::MILLI, kTestMillis,
                        three_days_later_ms, pool_));

  int64_t three_days_later_us = kTestMicros + 3 * 86400000000LL;
  EXPECT_EQ(3, EvalDiff("timestampdiffDay", arrow::TimeUnit::MICRO, kTestMicros,
                        three_days_later_us, pool_));

  int64_t three_days_later_ns = kTestNanos + 3 * 86400000000000LL;
  EXPECT_EQ(3, EvalDiff("timestampdiffDay", arrow::TimeUnit::NANO, kTestNanos,
                        three_days_later_ns, pool_));
}

// Helper: evaluate months_between(ts1, ts2) -> float64
static double EvalMonthsBetween(arrow::TimeUnit::type unit, int64_t ts1, int64_t ts2,
                                arrow::MemoryPool* pool) {
  auto ts_type = timestamp(unit);
  auto f1 = field("f1", ts_type);
  auto f2 = field("f2", ts_type);
  auto schema = arrow::schema({f1, f2});
  auto result_field = field("result", float64());

  auto n1 = TreeExprBuilder::MakeField(f1);
  auto n2 = TreeExprBuilder::MakeField(f2);
  auto func_node = TreeExprBuilder::MakeFunction("months_between", {n1, n2}, float64());
  auto expr = TreeExprBuilder::MakeExpression(func_node, result_field);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  auto a1 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts1}, {true});
  auto a2 = MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts2}, {true});
  auto in_batch = arrow::RecordBatch::Make(schema, 1, {a1, a2});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok());

  auto result_array = std::dynamic_pointer_cast<arrow::DoubleArray>(outputs.at(0));
  return result_array->Value(0);
}

TEST_F(DateTimeTestProjector, TestMonthsBetweenAcrossPrecisions) {
  // months_between returns the same value regardless of input precision.
  // Use millis as baseline.
  double base = EvalMonthsBetween(arrow::TimeUnit::MILLI, kTestMillis + 5270400000LL,
                                  kTestMillis, pool_);
  EXPECT_NEAR(base,
              EvalMonthsBetween(arrow::TimeUnit::MICRO, kTestMicros + 5270400000000LL,
                                kTestMicros, pool_),
              0.001);
  EXPECT_NEAR(base,
              EvalMonthsBetween(arrow::TimeUnit::NANO, kTestNanos + 5270400000000000LL,
                                kTestNanos, pool_),
              0.001);
}

// Helper: evaluate date_add/subtract(timestamp, int32) -> timestamp
static int64_t EvalDateArith(const std::string& func_name, arrow::TimeUnit::type unit,
                             int64_t ts_value, int32_t count, arrow::MemoryPool* pool) {
  auto ts_type = timestamp(unit);
  auto f_ts = field("ts", ts_type);
  auto f_count = field("count", int32());
  auto schema = arrow::schema({f_ts, f_count});
  auto result_field = field("result", ts_type);

  auto ts_node = TreeExprBuilder::MakeField(f_ts);
  auto count_node = TreeExprBuilder::MakeField(f_count);
  auto func_node =
      TreeExprBuilder::MakeFunction(func_name, {ts_node, count_node}, ts_type);
  auto expr = TreeExprBuilder::MakeExpression(func_node, result_field);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  auto a_ts =
      MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
  auto a_count = MakeArrowTypeArray<arrow::Int32Type, int32_t>(int32(), {count}, {true});
  auto in_batch = arrow::RecordBatch::Make(schema, 1, {a_ts, a_count});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok());

  auto result_array = std::dynamic_pointer_cast<arrow::TimestampArray>(outputs.at(0));
  return result_array->Value(0);
}

TEST_F(DateTimeTestProjector, TestDateAddSubtractAcrossPrecisions) {
  // date_add(ts, 3) adds 3 days. Sub-ms data must survive.
  int64_t three_days_us = 3 * 86400LL * 1000000;
  EXPECT_EQ(kTestMicros + three_days_us,
            EvalDateArith("date_add", arrow::TimeUnit::MICRO, kTestMicros, 3, pool_));

  int64_t three_days_ns = 3 * 86400LL * 1000000000;
  EXPECT_EQ(kTestNanos + three_days_ns,
            EvalDateArith("date_add", arrow::TimeUnit::NANO, kTestNanos, 3, pool_));

  // subtract(ts, 1) subtracts 1 day.
  int64_t one_day_us = 86400LL * 1000000;
  EXPECT_EQ(kTestMicros - one_day_us,
            EvalDateArith("subtract", arrow::TimeUnit::MICRO, kTestMicros, 1, pool_));
}

TEST_F(DateTimeTestProjector, TestTimestampaddMonthReversedArgMicros) {
  // timestampaddMonth(timestamp, int32) with micros — reversed arg order.
  // Use millis as ground truth.
  auto millis_result = EvalTimestampadd("timestampaddMonth", arrow::TimeUnit::MILLI, 2,
                                        kTestMillis, pool_);
  auto micros_result = EvalTimestampadd("timestampaddMonth", arrow::TimeUnit::MICRO, 2,
                                        kTestMicros, pool_);
  // Sub-ms data must survive.
  EXPECT_EQ(millis_result * 1000 + kSubMs, micros_result);
}

// castDATE: timestamp(us/ns) -> date64 (millis at midnight)
TEST_F(DateTimeTestProjector, TestCastDateAcrossPrecisions) {
  // 2021-06-15 14:30:45.123456789 -> should yield 2021-06-15 00:00:00 as date64 millis
  int64_t expected_date_millis = 1623715200000LL;  // 2021-06-15T00:00:00Z in millis

  auto eval_castdate = [this](arrow::TimeUnit::type unit, int64_t ts_value) -> int64_t {
    auto ts_type = timestamp(unit);
    auto f0 = field("f0", ts_type);
    auto schema = arrow::schema({f0});
    auto result_field = field("result", date64());
    auto expr = TreeExprBuilder::MakeExpression("castDATE", {f0}, result_field);

    std::shared_ptr<Projector> projector;
    auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
    EXPECT_TRUE(status.ok());

    auto in_array =
        MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
    auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array});

    arrow::ArrayVector outputs;
    status = projector->Evaluate(*in_batch, pool_, &outputs);
    EXPECT_TRUE(status.ok());
    auto result_array = std::dynamic_pointer_cast<arrow::Date64Array>(outputs.at(0));
    return result_array->Value(0);
  };

  EXPECT_EQ(expected_date_millis, eval_castdate(arrow::TimeUnit::MILLI, kTestMillis));
  EXPECT_EQ(expected_date_millis, eval_castdate(arrow::TimeUnit::MICRO, kTestMicros));
  EXPECT_EQ(expected_date_millis, eval_castdate(arrow::TimeUnit::NANO, kTestNanos));
}

// castTIME: timestamp(us/ns) -> time32(millis) — sub-ms truncated to millis
TEST_F(DateTimeTestProjector, TestCastTimeAcrossPrecisions) {
  // 2021-06-15 14:30:45.123456789 -> time-of-day = 14:30:45.123 = 52245123 ms
  int32_t expected_time_millis = 52245123;

  auto eval_casttime = [this](arrow::TimeUnit::type unit, int64_t ts_value) -> int32_t {
    auto ts_type = timestamp(unit);
    auto f0 = field("f0", ts_type);
    auto schema = arrow::schema({f0});
    auto result_field = field("result", time32(arrow::TimeUnit::MILLI));
    auto expr = TreeExprBuilder::MakeExpression("castTIME", {f0}, result_field);

    std::shared_ptr<Projector> projector;
    auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
    EXPECT_TRUE(status.ok());

    auto in_array =
        MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
    auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array});

    arrow::ArrayVector outputs;
    status = projector->Evaluate(*in_batch, pool_, &outputs);
    EXPECT_TRUE(status.ok());
    auto result_array = std::dynamic_pointer_cast<arrow::Time32Array>(outputs.at(0));
    return result_array->Value(0);
  };

  EXPECT_EQ(expected_time_millis, eval_casttime(arrow::TimeUnit::MILLI, kTestMillis));
  EXPECT_EQ(expected_time_millis, eval_casttime(arrow::TimeUnit::MICRO, kTestMicros));
  EXPECT_EQ(expected_time_millis, eval_casttime(arrow::TimeUnit::NANO, kTestNanos));
}

// Negative (pre-epoch) timestamps: verify extract and date_trunc produce correct
// results and that us/ns precisions match the millis baseline.
TEST_F(DateTimeTestProjector, TestNegativeTimestampPrecisions) {
  time_t epoch = Epoch();
  // 1960-03-15 06:30:00.000  (pre-epoch)
  int64_t neg_millis = MillisSince(epoch, 1960, 3, 15, 6, 30, 0, 0);
  int64_t neg_sub_ms = 456;  // sub-ms micros
  int64_t neg_sub_us = 789;  // sub-us nanos
  int64_t neg_micros = neg_millis * 1000 - neg_sub_ms;
  int64_t neg_nanos = neg_millis * 1000000 - neg_sub_ms * 1000 - neg_sub_us;

  // extractYear: all precisions must agree
  auto year_ms = EvalExtract("extractYear", arrow::TimeUnit::MILLI, neg_millis, pool_);
  EXPECT_EQ(year_ms,
            EvalExtract("extractYear", arrow::TimeUnit::MICRO, neg_micros, pool_));
  EXPECT_EQ(year_ms, EvalExtract("extractYear", arrow::TimeUnit::NANO, neg_nanos, pool_));

  // extractMonth: all precisions must agree
  auto month_ms = EvalExtract("extractMonth", arrow::TimeUnit::MILLI, neg_millis, pool_);
  EXPECT_EQ(month_ms,
            EvalExtract("extractMonth", arrow::TimeUnit::MICRO, neg_micros, pool_));
  EXPECT_EQ(month_ms,
            EvalExtract("extractMonth", arrow::TimeUnit::NANO, neg_nanos, pool_));

  // date_trunc_Day: us/ns results must equal millis result scaled up
  auto day_ms = EvalTrunc("date_trunc_Day", arrow::TimeUnit::MILLI, neg_millis, pool_);
  EXPECT_EQ(day_ms * 1000,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::MICRO, neg_micros, pool_));
  EXPECT_EQ(day_ms * 1000000,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::NANO, neg_nanos, pool_));
}

// Negative timestamps at calendar boundaries: SDiv truncates toward zero, so
// -456 us / 1000 = 0 millis (wrong, should be -1). This causes the precompiled
// function to see millis=0 (epoch) instead of millis=-1 (just before epoch),
// crossing a second/hour/day/year boundary.
TEST_F(DateTimeTestProjector, TestNegativeTimestampBoundaryCrossing) {
  // -456 us = 1969-12-31 23:59:59.999544
  // SDiv(-456, 1000) = 0  → precompiled sees epoch (1970-01-01 00:00:00)
  // Floor(-456 / 1000) = -1 → correct millis (1969-12-31 23:59:59.999)
  int64_t boundary_micros = -456;
  int64_t boundary_nanos = -456000 - 789;  // -456789 ns

  // extractHour: should be 23 (not 0)
  EXPECT_EQ(23,
            EvalExtract("extractHour", arrow::TimeUnit::MICRO, boundary_micros, pool_));
  EXPECT_EQ(23, EvalExtract("extractHour", arrow::TimeUnit::NANO, boundary_nanos, pool_));

  // extractYear: should be 1969 (not 1970)
  EXPECT_EQ(1969,
            EvalExtract("extractYear", arrow::TimeUnit::MICRO, boundary_micros, pool_));
  EXPECT_EQ(1969,
            EvalExtract("extractYear", arrow::TimeUnit::NANO, boundary_nanos, pool_));

  // Verify millis-level baseline: precompiled functions with millis = -1
  // (1969-12-31 23:59:59.999)
  int64_t boundary_millis = -1;
  auto day_millis =
      EvalExtract("extractDay", arrow::TimeUnit::MILLI, boundary_millis, pool_);
  auto hour_millis =
      EvalExtract("extractHour", arrow::TimeUnit::MILLI, boundary_millis, pool_);
  auto year_millis =
      EvalExtract("extractYear", arrow::TimeUnit::MILLI, boundary_millis, pool_);
  auto trunc_s_millis =
      EvalTrunc("date_trunc_Second", arrow::TimeUnit::MILLI, boundary_millis, pool_);
  auto trunc_d_millis =
      EvalTrunc("date_trunc_Day", arrow::TimeUnit::MILLI, boundary_millis, pool_);
  ARROW_LOG(WARNING) << "millis baseline: day=" << day_millis << " hour=" << hour_millis
                     << " year=" << year_millis << " trunc_s=" << trunc_s_millis
                     << " trunc_d=" << trunc_d_millis;

  // extractDay: should be 31 (not 1)
  EXPECT_EQ(31,
            EvalExtract("extractDay", arrow::TimeUnit::MICRO, boundary_micros, pool_));
  EXPECT_EQ(31, EvalExtract("extractDay", arrow::TimeUnit::NANO, boundary_nanos, pool_));

  // date_trunc_Second: 1969-12-31 23:59:59.000000 = -1000 ms = -1000000 us
  EXPECT_EQ(-1000000, EvalTrunc("date_trunc_Second", arrow::TimeUnit::MICRO,
                                boundary_micros, pool_));
  EXPECT_EQ(-1000000000,
            EvalTrunc("date_trunc_Second", arrow::TimeUnit::NANO, boundary_nanos, pool_));

  // date_trunc_Day: 1969-12-31 00:00:00 = -86400000 ms = -86400000000 us
  EXPECT_EQ(-86400000000LL,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::MICRO, boundary_micros, pool_));
  EXPECT_EQ(-86400000000000LL,
            EvalTrunc("date_trunc_Day", arrow::TimeUnit::NANO, boundary_nanos, pool_));
}

// castVARCHAR: verify sub-ms digits are included in the output string.
TEST_F(DateTimeTestProjector, TestCastVARCHARAcrossPrecisions) {
  auto eval_castVARCHAR = [this](arrow::TimeUnit::type unit,
                                 int64_t ts_value) -> std::string {
    auto ts_type = timestamp(unit);
    auto f0 = field("f0", ts_type);
    auto len_field = field("len", int64());
    auto schema = arrow::schema({f0, len_field});
    auto result_field = field("result", utf8());
    auto expr =
        TreeExprBuilder::MakeExpression("castVARCHAR", {f0, len_field}, result_field);

    std::shared_ptr<Projector> projector;
    auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
    EXPECT_TRUE(status.ok()) << status.ToString();

    auto in_array =
        MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
    auto len_array =
        MakeArrowTypeArray<arrow::Int64Type, int64_t>(int64(), {100}, {true});
    auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array, len_array});

    arrow::ArrayVector outputs;
    status = projector->Evaluate(*in_batch, pool_, &outputs);
    EXPECT_TRUE(status.ok()) << status.ToString();
    auto result_array = std::dynamic_pointer_cast<arrow::StringArray>(outputs.at(0));
    return result_array->GetString(0);
  };

  // kTest* = 2021-06-15 14:30:45.123456789
  std::string expected_ms = "2021-06-15 14:30:45.123";
  std::string expected_us = "2021-06-15 14:30:45.123456";
  std::string expected_ns = "2021-06-15 14:30:45.123456789";

  EXPECT_EQ(expected_ms, eval_castVARCHAR(arrow::TimeUnit::MILLI, kTestMillis));
  EXPECT_EQ(expected_us, eval_castVARCHAR(arrow::TimeUnit::MICRO, kTestMicros));
  EXPECT_EQ(expected_ns, eval_castVARCHAR(arrow::TimeUnit::NANO, kTestNanos));
}

// castVARCHAR with length truncation: sub-ms digits should be omitted when the
// length limit doesn't allow them.
TEST_F(DateTimeTestProjector, TestCastVARCHARTruncation) {
  auto eval_truncated = [this](arrow::TimeUnit::type unit, int64_t ts_value,
                               int64_t max_len) -> std::string {
    auto ts_type = timestamp(unit);
    auto f0 = field("f0", ts_type);
    auto len_field = field("len", int64());
    auto schema = arrow::schema({f0, len_field});
    auto result_field = field("result", utf8());
    auto expr =
        TreeExprBuilder::MakeExpression("castVARCHAR", {f0, len_field}, result_field);

    std::shared_ptr<Projector> projector;
    auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
    EXPECT_TRUE(status.ok()) << status.ToString();

    auto in_array =
        MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
    auto len_array =
        MakeArrowTypeArray<arrow::Int64Type, int64_t>(int64(), {max_len}, {true});
    auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array, len_array});

    arrow::ArrayVector outputs;
    status = projector->Evaluate(*in_batch, pool_, &outputs);
    EXPECT_TRUE(status.ok()) << status.ToString();
    auto result_array = std::dynamic_pointer_cast<arrow::StringArray>(outputs.at(0));
    return result_array->GetString(0);
  };

  // Full output for ns = "2021-06-15 14:30:45.123456789" (29 chars)
  // Truncate to 23 chars → millis only: "2021-06-15 14:30:45.123"
  EXPECT_EQ("2021-06-15 14:30:45.123",
            eval_truncated(arrow::TimeUnit::NANO, kTestNanos, 23));
  // Truncate to 26 chars → us precision: "2021-06-15 14:30:45.123456"
  EXPECT_EQ("2021-06-15 14:30:45.123456",
            eval_truncated(arrow::TimeUnit::NANO, kTestNanos, 26));
  // Truncate to 20 chars → no fractional seconds
  EXPECT_EQ("2021-06-15 14:30:45.",
            eval_truncated(arrow::TimeUnit::NANO, kTestNanos, 20));
}

// Negative (pre-epoch) castVARCHAR: floor-division remainder must produce
// correct sub-millisecond digits even for negative timestamps.
TEST_F(DateTimeTestProjector, TestCastVARCHARNegativeTimestamp) {
  auto eval_castVARCHAR = [this](arrow::TimeUnit::type unit,
                                 int64_t ts_value) -> std::string {
    auto ts_type = timestamp(unit);
    auto f0 = field("f0", ts_type);
    auto len_field = field("len", int64());
    auto schema = arrow::schema({f0, len_field});
    auto result_field = field("result", utf8());
    auto expr =
        TreeExprBuilder::MakeExpression("castVARCHAR", {f0, len_field}, result_field);

    std::shared_ptr<Projector> projector;
    auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
    EXPECT_TRUE(status.ok()) << status.ToString();

    auto in_array =
        MakeArrowTypeArray<arrow::TimestampType, int64_t>(ts_type, {ts_value}, {true});
    auto len_array =
        MakeArrowTypeArray<arrow::Int64Type, int64_t>(int64(), {100}, {true});
    auto in_batch = arrow::RecordBatch::Make(schema, 1, {in_array, len_array});

    arrow::ArrayVector outputs;
    status = projector->Evaluate(*in_batch, pool_, &outputs);
    EXPECT_TRUE(status.ok()) << status.ToString();
    auto result_array = std::dynamic_pointer_cast<arrow::StringArray>(outputs.at(0));
    return result_array->GetString(0);
  };

  // -1 microsecond = 1969-12-31 23:59:59.999999
  EXPECT_EQ("1969-12-31 23:59:59.999999", eval_castVARCHAR(arrow::TimeUnit::MICRO, -1));
  // -1 nanosecond = 1969-12-31 23:59:59.999999999
  EXPECT_EQ("1969-12-31 23:59:59.999999999", eval_castVARCHAR(arrow::TimeUnit::NANO, -1));
  // -456 microseconds = 1969-12-31 23:59:59.999544
  EXPECT_EQ("1969-12-31 23:59:59.999544", eval_castVARCHAR(arrow::TimeUnit::MICRO, -456));
  // -456789 nanoseconds = 1969-12-31 23:59:59.999543211
  EXPECT_EQ("1969-12-31 23:59:59.999543211",
            eval_castVARCHAR(arrow::TimeUnit::NANO, -456789));
}

// NOTE: TIME type handling
// time32 uses TimeUnit::SECOND or MILLISECOND.
// time64 uses TimeUnit::MICROSECOND or NANOSECOND.
// Gandiva's time functions (extractHour/Minute/Second on time32) operate on millis.
// time64[us]/time64[ns] will need the same TimestampIR treatment: convert to millis
// before calling precompiled functions. The pattern is identical to the extract wrappers.

}  // namespace gandiva
