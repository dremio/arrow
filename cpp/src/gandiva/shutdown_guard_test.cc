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

#include "gandiva/shutdown_guard.h"

#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

namespace gandiva {

using std::chrono::milliseconds;

// PreShutdown() is one-way for the life of the process, so the tests in this file run in
// order: the ones that need a live state come first and the last one starts the shutdown.

TEST(TestBuildScope, NestedScopesAreAdmitted) {
  BuildScope outer;
  ASSERT_TRUE(outer.entered());
  {
    BuildScope nested;
    EXPECT_TRUE(nested.entered());
  }
  BuildScope second_nested;
  EXPECT_TRUE(second_nested.entered());
}

TEST(TestBuildScope, PreShutdownFromInsideBuildFails) {
  BuildScope scope;
  ASSERT_TRUE(scope.entered());

  auto status = BuildScope::PreShutdown(milliseconds(10));
  EXPECT_TRUE(status.IsInvalid()) << status.ToString();
}

TEST(TestBuildScope, PreShutdownWaitsForRunningBuild) {
  std::promise<void> holding;
  std::promise<void> shutdown_started;
  std::promise<void> nested_checked;
  std::promise<void> release;
  bool nested_entered = false;

  std::thread builder([&] {
    BuildScope outer;
    EXPECT_TRUE(outer.entered());
    holding.set_value();

    shutdown_started.get_future().wait();
    BuildScope nested;
    nested_entered = nested.entered();
    nested_checked.set_value();

    release.get_future().wait();
  });
  holding.get_future().wait();

  auto timed_out = BuildScope::PreShutdown(milliseconds(50));
  EXPECT_TRUE(timed_out.IsIOError()) << timed_out.ToString();

  // the shutdown has started: the running build may still nest, new builds are rejected
  shutdown_started.set_value();
  nested_checked.get_future().wait();
  EXPECT_TRUE(nested_entered);

  BuildScope rejected;
  EXPECT_FALSE(rejected.entered());

  std::thread releaser([&] {
    std::this_thread::sleep_for(milliseconds(100));
    release.set_value();
  });
  auto status = BuildScope::PreShutdown(std::chrono::seconds(30));
  EXPECT_TRUE(status.ok()) << status.ToString();

  releaser.join();
  builder.join();
}

}  // namespace gandiva
