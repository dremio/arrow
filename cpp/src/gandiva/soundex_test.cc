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

#include <cstdint>
#include <string>

#include <gtest/gtest.h>

#include "gandiva/execution_context.h"
#include "gandiva/precompiled/types.h"

namespace gandiva {

TEST(Soundex, Basic) {
  gandiva::ExecutionContext ctx;
  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  int32_t out_len = 0;
  bool out_valid = false;

  const char* out = soundex_utf8(ctx_ptr, "Robert", 6, true, &out_valid, &out_len);
  EXPECT_TRUE(out_valid);
  EXPECT_EQ(out_len, 4);
  EXPECT_EQ(std::string(out, out_len), "R163");
}

TEST(Soundex, IgnoresNonAlphabetic) {
  gandiva::ExecutionContext ctx;
  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  int32_t out_len = 0;
  bool out_valid = false;

  const char* out = soundex_utf8(ctx_ptr, "r-O-b-E-r-T", 11, true, &out_valid, &out_len);
  EXPECT_TRUE(out_valid);
  EXPECT_EQ(std::string(out, out_len), "R163");
}

TEST(Soundex, NoAlphabeticReturnsNull) {
  gandiva::ExecutionContext ctx;
  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  int32_t out_len = 0;
  bool out_valid = true;

  const char* out = soundex_utf8(ctx_ptr, "123456789", 9, true, &out_valid, &out_len);
  EXPECT_FALSE(out_valid);
  EXPECT_EQ(out_len, 0);
  EXPECT_EQ(std::string(out, out_len), "");
}

TEST(Soundex, NullInput) {
  gandiva::ExecutionContext ctx;
  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  int32_t out_len = 0;
  bool out_valid = true;

  const char* out = soundex_utf8(ctx_ptr, nullptr, 0, false, &out_valid, &out_len);
  EXPECT_FALSE(out_valid);
  EXPECT_EQ(out_len, 0);
  EXPECT_EQ(std::string(out, out_len), "");
}

}  // namespace gandiva
