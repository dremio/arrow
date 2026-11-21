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

#include "gandiva/cache.h"
#include "arrow/testing/gtest_util.h"
#include "arrow/util/io_util.h"
#include "arrow/util/logging.h"

#include <gtest/gtest.h>

#include "gandiva/configuration.h"
#include "gandiva/expression_cache_key.h"
#include "gandiva/tree_expr_builder.h"

namespace gandiva {

class TestCacheKey {
 public:
  explicit TestCacheKey(int value) : value_(value) {}
  std::size_t Hash() const { return value_; }
  bool operator==(const TestCacheKey& other) const { return value_ == other.value_; }

 private:
  int value_;
};

TEST(TestCache, TestGetPut) {
  Cache<TestCacheKey, std::shared_ptr<std::string>> cache(2);
  auto hello = std::make_shared<std::string>("hello");
  cache.PutObjectCode(TestCacheKey(1), hello);
  auto world = std::make_shared<std::string>("world");
  cache.PutObjectCode(TestCacheKey(2), world);
  ASSERT_EQ(cache.GetObjectCode(TestCacheKey(1)), hello);
  ASSERT_EQ(cache.GetObjectCode(TestCacheKey(2)), world);
}

namespace {
constexpr auto cache_capacity_env_var = "GANDIVA_CACHE_SIZE";
constexpr auto default_cache_capacity = 5000;
}  // namespace

TEST(TestCache, TestGetCacheCapacityDefault) {
  ASSERT_EQ(GetCacheCapacity(), default_cache_capacity);
}

TEST(TestCache, TestGetCacheCapacityEnvVar) {
  using ::arrow::EnvVarGuard;

  // Empty.
  {
    EnvVarGuard guard(cache_capacity_env_var, "");
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), default_cache_capacity);
  }

  // Non-number.
  {
    EnvVarGuard guard(cache_capacity_env_var, "invalid");
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), default_cache_capacity);
  }

  // Number with invalid suffix.
  {
    EnvVarGuard guard(cache_capacity_env_var, "42MB");
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), default_cache_capacity);
  }

  // Valid positive number.
  {
    EnvVarGuard guard(cache_capacity_env_var, "42");
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), 42);
  }

  // Int max.
  {
    auto str = std::to_string(std::numeric_limits<int>::max());
    EnvVarGuard guard(cache_capacity_env_var, str.c_str());
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), std::numeric_limits<int>::max());
  }

  // Zero.
  {
    EnvVarGuard guard(cache_capacity_env_var, "0");
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), default_cache_capacity);
  }

  // Negative number.
  {
    EnvVarGuard guard(cache_capacity_env_var, "-1");
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), default_cache_capacity);
  }

  // Over int max.
  {
    auto str = std::to_string(static_cast<int64_t>(std::numeric_limits<int>::max()) + 1);
    EnvVarGuard guard(cache_capacity_env_var, str.c_str());
    ASSERT_EQ(internal::GetCacheCapacityFromEnvVar(), default_cache_capacity);
  }
}

// Two distinct Configuration instances with identical content must produce equal
// ExpressionCacheKeys, so the compiled-expression cache hits instead of recompiling.
// Regression test for the key comparing the configuration shared_ptr by identity.
TEST(TestCache, TestExpressionCacheKeyConfigByValue) {
  auto f0 = arrow::field("f0", arrow::int32());
  auto f1 = arrow::field("f1", arrow::int32());
  auto schema = arrow::schema({f0, f1});
  auto expr =
      TreeExprBuilder::MakeExpression("add", {f0, f1}, arrow::field("out", arrow::int32()));
  ExpressionVector exprs = {expr};
  const auto mode = SelectionVector::Mode::MODE_NONE;

  // Distinct instances, identical content.
  auto cfg1 = std::make_shared<Configuration>(true);
  auto cfg2 = std::make_shared<Configuration>(true);
  ASSERT_NE(cfg1, cfg2);               // different pointers
  ASSERT_EQ(*cfg1, *cfg2);             // equal content

  ExpressionCacheKey k1(schema, cfg1, exprs, mode);
  ExpressionCacheKey k2(schema, cfg2, exprs, mode);
  ASSERT_EQ(k1.Hash(), k2.Hash());
  ASSERT_TRUE(k1 == k2);               // fails if configuration is compared by identity

  // Different configuration content must still produce a different key.
  auto cfg3 = std::make_shared<Configuration>(false);  // optimize = false
  ExpressionCacheKey k3(schema, cfg3, exprs, mode);
  ASSERT_FALSE(k1 == k3);
}

}  // namespace gandiva
