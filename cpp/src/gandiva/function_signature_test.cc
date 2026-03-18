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

#include "gandiva/function_signature.h"

#include <memory>

#include <gtest/gtest.h>

namespace gandiva {

class TestFunctionSignature : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // Use make_shared so these are distinct from the static instances returned
    // by e.g. arrow::int32()
    local_i32_type_ = std::make_shared<arrow::Int32Type>();
    local_i64_type_ = std::make_shared<arrow::Int64Type>();
    local_date32_type_ = std::make_shared<arrow::Date32Type>();
  }

  virtual void TearDown() {
    local_i32_type_.reset();
    local_i64_type_.reset();
    local_date32_type_.reset();
  }

  // virtual void TearDown() {}
  DataTypePtr local_i32_type_;
  DataTypePtr local_i64_type_;
  DataTypePtr local_date32_type_;
};

TEST_F(TestFunctionSignature, TestToString) {
  EXPECT_EQ(
      FunctionSignature("myfunc", {arrow::int32(), arrow::float32()}, arrow::float64())
          .ToString(),
      "double myfunc(int32, float)");
}

TEST_F(TestFunctionSignature, TestEqualsName) {
  EXPECT_EQ(FunctionSignature("add", {arrow::int32()}, arrow::int32()),
            FunctionSignature("add", {arrow::int32()}, arrow::int32()));

  EXPECT_EQ(FunctionSignature("add", {arrow::int32()}, arrow::int64()),
            FunctionSignature("add", {local_i32_type_}, local_i64_type_));

  EXPECT_FALSE(FunctionSignature("add", {arrow::int32()}, arrow::int32()) ==
               FunctionSignature("sub", {arrow::int32()}, arrow::int32()));

  EXPECT_EQ(FunctionSignature("extractDay", {arrow::int64()}, arrow::int64()),
            FunctionSignature("extractday", {arrow::int64()}, arrow::int64()));

  EXPECT_EQ(
      FunctionSignature("castVARCHAR", {arrow::utf8(), arrow::int64()}, arrow::utf8()),
      FunctionSignature("castvarchar", {arrow::utf8(), arrow::int64()}, arrow::utf8()));
}

TEST_F(TestFunctionSignature, TestEqualsParamCount) {
  EXPECT_FALSE(
      FunctionSignature("add", {arrow::int32(), arrow::int32()}, arrow::int32()) ==
      FunctionSignature("add", {arrow::int32()}, arrow::int32()));
}

TEST_F(TestFunctionSignature, TestEqualsParamValue) {
  EXPECT_FALSE(FunctionSignature("add", {arrow::int32()}, arrow::int32()) ==
               FunctionSignature("add", {arrow::int64()}, arrow::int32()));

  EXPECT_FALSE(
      FunctionSignature("add", {arrow::int32()}, arrow::int32()) ==
      FunctionSignature("add", {arrow::float32(), arrow::float32()}, arrow::int32()));

  EXPECT_FALSE(
      FunctionSignature("add", {arrow::int32(), arrow::int64()}, arrow::int32()) ==
      FunctionSignature("add", {arrow::int64(), arrow::int32()}, arrow::int32()));

  EXPECT_EQ(FunctionSignature("extract_month", {arrow::date32()}, arrow::int64()),
            FunctionSignature("extract_month", {local_date32_type_}, local_i64_type_));

  EXPECT_FALSE(FunctionSignature("extract_month", {arrow::date32()}, arrow::int64()) ==
               FunctionSignature("extract_month", {arrow::date64()}, arrow::date32()));
}

TEST_F(TestFunctionSignature, TestEqualsReturn) {
  EXPECT_FALSE(FunctionSignature("add", {arrow::int32()}, arrow::int64()) ==
               FunctionSignature("add", {arrow::int32()}, arrow::int32()));
}

TEST_F(TestFunctionSignature, TestHash) {
  FunctionSignature f1("add", {arrow::int32(), arrow::int32()}, arrow::int64());
  FunctionSignature f2("add", {local_i32_type_, local_i32_type_}, local_i64_type_);
  EXPECT_EQ(f1.Hash(), f2.Hash());

  FunctionSignature f3("extractDay", {arrow::int64()}, arrow::int64());
  FunctionSignature f4("extractday", {arrow::int64()}, arrow::int64());
  EXPECT_EQ(f3.Hash(), f4.Hash());
}

TEST_F(TestFunctionSignature, TestTimestampPrecisionHash) {
  // Different timestamp precisions should have different hashes
  FunctionSignature ts_sec("extractYear",
                           {arrow::timestamp(arrow::TimeUnit::SECOND)},
                           arrow::int64());
  FunctionSignature ts_ms("extractYear",
                          {arrow::timestamp(arrow::TimeUnit::MILLI)},
                          arrow::int64());
  FunctionSignature ts_us("extractYear",
                          {arrow::timestamp(arrow::TimeUnit::MICRO)},
                          arrow::int64());
  FunctionSignature ts_ns("extractYear",
                          {arrow::timestamp(arrow::TimeUnit::NANO)},
                          arrow::int64());

  // All should have different hashes
  EXPECT_NE(ts_sec.Hash(), ts_ms.Hash());
  EXPECT_NE(ts_sec.Hash(), ts_us.Hash());
  EXPECT_NE(ts_sec.Hash(), ts_ns.Hash());
  EXPECT_NE(ts_ms.Hash(), ts_us.Hash());
  EXPECT_NE(ts_ms.Hash(), ts_ns.Hash());
  EXPECT_NE(ts_us.Hash(), ts_ns.Hash());

  // Same precision should have same hash
  FunctionSignature ts_sec2("extractYear",
                            {arrow::timestamp(arrow::TimeUnit::SECOND)},
                            arrow::int64());
  EXPECT_EQ(ts_sec.Hash(), ts_sec2.Hash());
}

TEST_F(TestFunctionSignature, TestTimestampPrecisionEquals) {
  // Different timestamp precisions should NOT be equal
  FunctionSignature ts_sec("extractYear",
                           {arrow::timestamp(arrow::TimeUnit::SECOND)},
                           arrow::int64());
  FunctionSignature ts_ms("extractYear",
                          {arrow::timestamp(arrow::TimeUnit::MILLI)},
                          arrow::int64());

  EXPECT_FALSE(ts_sec == ts_ms);

  // Same precision should be equal
  FunctionSignature ts_sec2("extractYear",
                            {arrow::timestamp(arrow::TimeUnit::SECOND)},
                            arrow::int64());
  EXPECT_EQ(ts_sec, ts_sec2);
}

TEST_F(TestFunctionSignature, TestTimestampReturnTypeHash) {
  // Return type precision should also be distinguished
  FunctionSignature ret_ms("castTimestamp",
                           {arrow::utf8()},
                           arrow::timestamp(arrow::TimeUnit::MILLI));
  FunctionSignature ret_ns("castTimestamp",
                           {arrow::utf8()},
                           arrow::timestamp(arrow::TimeUnit::NANO));

  EXPECT_NE(ret_ms.Hash(), ret_ns.Hash());
  EXPECT_FALSE(ret_ms == ret_ns);
}

TEST_F(TestFunctionSignature, TestTime32PrecisionHash) {
  // Time32 types should also have precision-aware hashing
  FunctionSignature t32_sec("extractHour",
                            {arrow::time32(arrow::TimeUnit::SECOND)},
                            arrow::int64());
  FunctionSignature t32_ms("extractHour",
                           {arrow::time32(arrow::TimeUnit::MILLI)},
                           arrow::int64());

  EXPECT_NE(t32_sec.Hash(), t32_ms.Hash());
  EXPECT_FALSE(t32_sec == t32_ms);
}

TEST_F(TestFunctionSignature, TestTime64PrecisionHash) {
  // Time64 types should also have precision-aware hashing
  FunctionSignature t64_us("extractHour",
                           {arrow::time64(arrow::TimeUnit::MICRO)},
                           arrow::int64());
  FunctionSignature t64_ns("extractHour",
                           {arrow::time64(arrow::TimeUnit::NANO)},
                           arrow::int64());

  EXPECT_NE(t64_us.Hash(), t64_ns.Hash());
  EXPECT_FALSE(t64_us == t64_ns);
}

}  // namespace gandiva
