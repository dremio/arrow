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

#include <mutex>

namespace gandiva {

BuildScope::State& BuildScope::state() {
  static State* s = new State;  // leaked on purpose, thread-safe init (C++11)
  return *s;
}

int& BuildScope::Depth() {
  thread_local int depth = 0;  // trivially destructible, safe during exit
  return depth;
}

BuildScope::BuildScope() {
  int& depth = Depth();
  if (depth > 0) {  // nested: the outermost scope already holds the count
    ++depth;
    entered_ = true;
    return;
  }
  entered_ = TryAcquire();
  if (entered_) {
    depth = 1;
  }
}

BuildScope::~BuildScope() {
  if (!entered_) {
    return;
  }
  if (--Depth() == 0) {  // only the outermost scope releases
    Release();
  }
}

arrow::Status BuildScope::PreShutdown(std::chrono::milliseconds timeout) {
  if (Depth() > 0) {
    return arrow::Status::Invalid(
        "PreShutdown called from inside a Gandiva build; it would wait for itself");
  }
  auto& s = state();

  // acq_rel:
  //  - acquire half: if builds already finished, we synchronize with their release in
  //    Release(), so their work is visible to us even if the count is already zero and
  //    we never need to sleep.
  //  - release half: not strictly required for correctness. It is kept so that the
  //    request "shutdown started" is published to threads that later observe the bit,
  //    and it costs nothing on x86 and little on ARM.
  s.word.fetch_or(kShutdownBit, std::memory_order_acq_rel);

  // The mutex is only used to sleep and wake. It is NOT what protects the count.
  std::unique_lock<std::mutex> lock(s.mtx);
  bool quiescent = s.cv.wait_for(lock, timeout, [&s] {
    // acquire: pairs with the release (fetch_sub) in Release(). When we read a count of
    // zero, all work done by the finished builds is visible to us. This is the load that
    // makes it safe to run static destructors after PreShutdown returns OK.
    return (s.word.load(std::memory_order_acquire) & kCountMask) == 0;
  });
  if (!quiescent) {
    // relaxed: diagnostic only. We are returning an error and no other data depends on
    // this value, so no ordering is needed.
    return arrow::Status::IOError("Timed out waiting for ",
                                  s.word.load(std::memory_order_relaxed) & kCountMask,
                                  " Gandiva build(s) in flight");
  }
  return arrow::Status::OK();
}

bool BuildScope::TryAcquire() {
  auto& s = state();

  // relaxed: this is only a first guess for the CAS below. The CAS re-validates it
  // atomically, so a stale value is harmless (the CAS fails and refreshes `v`).
  uint32_t v = s.word.load(std::memory_order_relaxed);
  do {
    if (v & kShutdownBit) {
      return false;
    }
    // Check-and-increment in ONE atomic step: we only increment if the word is still
    // exactly `v` (bit not set). If PreShutdown sets the bit in between, the CAS fails.
    //
    // success = acq_rel:
    //  - acquire half: the build body must not be reordered before this point. This is
    //    the "lock acquired" side, and it also makes us see the work of builds that
    //    released earlier. This is the half that matters.
    //  - release half: not strictly needed here, kept as a harmless superset so the
    //    operation reads as a full read-modify-write synchronization point.
    //
    // failure = relaxed: a failed CAS writes nothing. It only reloads `v`, and the loop
    // re-checks the shutdown bit, so no ordering is needed.
    //
    // compare_exchange_weak: may fail spuriously (on ARM/POWER), which is fine inside
    // a retry loop and can generate cheaper code than compare_exchange_strong.
  } while (!s.word.compare_exchange_weak(v, v + 1, std::memory_order_acq_rel,
                                         std::memory_order_relaxed));
  return true;
}

void BuildScope::Release() {
  auto& s = state();

  // acq_rel (the release half is the important one):
  //  - release: publishes "this build is completely finished". Everything the build did
  //    before this point is visible to a thread that later acquire-loads this word and
  //    sees the decremented value, which is exactly what PreShutdown does. Without it,
  //    the count could reach zero while the build's memory effects were still not
  //    visible to the shutdown thread.
  //  - acquire: not strictly needed here, kept for symmetry with a lock/unlock pair.
  //
  // fetch_sub returns the value BEFORE the decrement, so we know exactly whether we were
  // the last build and whether the shutdown bit was set at that moment.
  uint32_t prev = s.word.fetch_sub(1, std::memory_order_acq_rel);

  if ((prev & kShutdownBit) && (prev & kCountMask) == 1) {
    std::lock_guard<std::mutex> lock(s.mtx);
    s.cv.notify_all();
  }
}

}  // namespace gandiva