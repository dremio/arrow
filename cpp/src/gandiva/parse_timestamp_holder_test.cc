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

#include <memory>

#include "arrow/testing/gtest_util.h"

#include "gandiva/execution_context.h"
#include "gandiva/parse_timestamp_holder.h"

#include <gtest/gtest.h>

namespace gandiva {

class TestParseTimestampHolder : public ::testing::Test {
 protected:
  ExecutionContext execution_context_;
};

TEST_F(TestParseTimestampHolder, TestSimpleTimestamp) {
  EXPECT_OK_AND_ASSIGN(auto holder,
                       ParseTimestampHolder::Make("YYYY-MM-DD HH24:MI:SS"));

  auto& parse_timestamp = *holder;
  bool out_valid;

  std::string s("1986-12-01 01:01:01");
  int64_t millis_since_epoch =
      parse_timestamp(&execution_context_, s.data(), (int)s.length(), true, &out_valid);
  EXPECT_TRUE(out_valid);
  EXPECT_EQ(millis_since_epoch, 533782861000);

  // allow trailing chars (fractional seconds not present in format)
  s = std::string("1986-12-01 01:01:01.11");
  millis_since_epoch =
      parse_timestamp(&execution_context_, s.data(), (int)s.length(), true, &out_valid);
  EXPECT_TRUE(out_valid);
  EXPECT_EQ(millis_since_epoch, 533782861000);
}

TEST_F(TestParseTimestampHolder, TestQuotedTextInFormat) {
  EXPECT_OK_AND_ASSIGN(auto holder,
                       ParseTimestampHolder::Make("YYYY-MM-DD\"T\"HH24:MI:SS"));

  auto& parse_timestamp = *holder;
  bool out_valid;

  std::string s("1986-12-01T01:01:01");
  int64_t millis_since_epoch =
      parse_timestamp(&execution_context_, s.data(), (int)s.length(), true, &out_valid);
  EXPECT_TRUE(out_valid);
  EXPECT_EQ(millis_since_epoch, 533782861000);
}

TEST_F(TestParseTimestampHolder, TestParseError) {
  EXPECT_OK_AND_ASSIGN(auto holder,
                       ParseTimestampHolder::Make("YYYY-MM-DD HH24:MI:SS"));

  auto& parse_timestamp = *holder;
  bool out_valid;

  std::string s("1986-01-40 01:01:01");
  int64_t millis_since_epoch =
      parse_timestamp(&execution_context_, s.data(), (int)s.length(), true, &out_valid);
  EXPECT_FALSE(out_valid);
  EXPECT_EQ(millis_since_epoch, 0);

  std::string expected_error =
      "Error parsing value 1986-01-40 01:01:01 for given format";
  EXPECT_TRUE(execution_context_.get_error().find(expected_error) != std::string::npos);

  // not valid should not return error
  execution_context_.Reset();
  millis_since_epoch =
      parse_timestamp(&execution_context_, "nullptr", 7, false, &out_valid);
  EXPECT_EQ(millis_since_epoch, 0);
  EXPECT_FALSE(execution_context_.has_error());
}

TEST_F(TestParseTimestampHolder, TestMakeError) {
  // reject unknown patterns.
  ASSERT_RAISES(Invalid,
                ParseTimestampHolder::Make("YYYY-MM-DD HH24:MI:SS tzo").status());
}

}  // namespace gandiva
