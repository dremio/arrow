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

#include "gandiva/date_format_holder.h"

#include <chrono>
#include <string>

#include <gtest/gtest.h>

#include "arrow/testing/gtest_util.h"
#include "arrow/vendored/datetime.h"

namespace gandiva {

TEST(TestDateFormatHolder, BasicFormatting) {
  ASSERT_OK_AND_ASSIGN(auto holder,
                       DateFormatTimestampHolder::Make("yyyy-MM-dd HH:mm:ss"));

  using arrow_vendored::date::sys_days;
  using arrow_vendored::date::year;
  using std::chrono::hours;
  using std::chrono::milliseconds;
  using std::chrono::minutes;
  using std::chrono::seconds;

  const auto tp = sys_days{year{2023} / 1 / 15} + hours{13} + minutes{45} + seconds{59};
  const int64_t millis_since_epoch =
      std::chrono::duration_cast<milliseconds>(tp.time_since_epoch()).count();

  ExecutionContext ctx;
  bool out_valid = false;
  int32_t out_len = 0;

  const char* out = (*holder)(&ctx, millis_since_epoch, /*in_valid=*/true, &out_valid,
                             &out_len);

  ASSERT_TRUE(out_valid);
  ASSERT_EQ(out_len, 19);
  ASSERT_EQ(std::string(out, out_len), "2023-01-15 13:45:59");
}

TEST(TestDateFormatHolder, QuotedText) {
  ASSERT_OK_AND_ASSIGN(auto holder,
                       DateFormatTimestampHolder::Make("yyyy-MM-dd'T'HH:mm:ss"));

  using arrow_vendored::date::sys_days;
  using arrow_vendored::date::year;
  using std::chrono::hours;
  using std::chrono::milliseconds;
  using std::chrono::minutes;
  using std::chrono::seconds;

  const auto tp = sys_days{year{2023} / 1 / 15} + hours{13} + minutes{45} + seconds{59};
  const int64_t millis_since_epoch =
      std::chrono::duration_cast<milliseconds>(tp.time_since_epoch()).count();

  ExecutionContext ctx;
  bool out_valid = false;
  int32_t out_len = 0;

  const char* out = (*holder)(&ctx, millis_since_epoch, /*in_valid=*/true, &out_valid,
                             &out_len);

  ASSERT_TRUE(out_valid);
  ASSERT_EQ(out_len, 19);
  ASSERT_EQ(std::string(out, out_len), "2023-01-15T13:45:59");
}

TEST(TestDateFormatHolder, InvalidInput) {
  ASSERT_OK_AND_ASSIGN(auto holder, DateFormatTimestampHolder::Make("yyyy-MM-dd"));

  ExecutionContext ctx;
  bool out_valid = true;
  int32_t out_len = 123;

  const char* out = (*holder)(&ctx, /*timestamp_millis=*/0, /*in_valid=*/false,
                             &out_valid, &out_len);

  ASSERT_FALSE(out_valid);
  ASSERT_EQ(out_len, 0);
  ASSERT_EQ(std::string(out), "");
}

TEST(TestDateFormatHolder, InvalidFormat) {
  ASSERT_RAISES(Invalid, DateFormatTimestampHolder::Make("yyyy-MM-dd QQ"));
}

}  // namespace gandiva
