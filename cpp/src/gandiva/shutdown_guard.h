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

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>

#include "arrow/status.h"

#include "gandiva/visibility.h"

namespace gandiva {

/// Held for the duration of a Projector::Make or Filter::Make.
///
/// A compile cannot be interrupted, and exit() destroys process-wide state (static
/// destructors) that a running compile still reads. The scopes count the compiles in
/// flight, so that the embedder can call PreShutdown() before the process exits to reject
/// new builds and wait for the running ones.
///
/// Scopes may be nested on the same thread: only the outermost scope is counted, and
/// nested scopes are always admitted, since the outermost one already keeps the build
/// "in flight". A scope must be destroyed on the thread that created it.
class GANDIVA_EXPORT BuildScope {
 public:
  BuildScope();
  ~BuildScope();

  BuildScope(const BuildScope&) = delete;
  BuildScope& operator=(const BuildScope&) = delete;
  BuildScope(BuildScope&&) = delete;
  BuildScope& operator=(BuildScope&&) = delete;

  /// False if the process is shutting down and the build must not start.
  bool entered() const { return entered_; }

  /// Rejects new builds and waits until no build is in flight, or the timeout expires.
  /// Returns Status::OK() when quiescent, Status::IOError on timeout, and
  /// Status::Invalid if called from a thread that is itself inside a build (it would
  /// wait for itself).
  static arrow::Status PreShutdown(std::chrono::milliseconds timeout);

 private:
  static constexpr std::uint32_t kShutdownBit = 0x80000000u;
  static constexpr std::uint32_t kCountMask = 0x7FFFFFFFu;

  struct State {
    // bit 31 = shutting down, low 31 bits = outermost builds in flight.
    // Flag and count share one word so "check flag + increment" is a single atomic step.
    std::atomic<std::uint32_t> word{0};
    // Only used to put PreShutdown to sleep and wake it up.
    std::mutex mtx;
    std::condition_variable cv;
  };

  // Leaked on purpose: builds still running at exit must find it alive.
  // Function-local static avoids static initialization order problems.
  static State& state();

  // Per-thread nesting depth of live, entered scopes.
  static int& Depth();

  /// Counts an outermost build as in flight. Fails, leaving the count unchanged, once
  /// PreShutdown has started.
  static bool TryAcquire();
  static void Release();

  bool entered_ = false;
};

}  // namespace gandiva