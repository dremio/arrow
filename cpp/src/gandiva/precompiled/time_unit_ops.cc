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

// Precompiled time64 (time-of-day) functions.
//
// Gandiva registers time64 signatures with time64[us] (see function_registry_common.h).
// The unsuffixed functions below therefore take microseconds-of-day, and the _ns
// variants take nanoseconds-of-day. LLVMGenerator::ResolveTimePcName() remaps a call
// to the _ns variant when the expression's time64 argument has NANO unit, the same
// way timestamp[us/ns] calls are remapped to the variants in timestamp_unit_ops.cc.
//
// Values are offsets within a single day (0 .. 86'399'999'999 for micros,
// 0 .. 86'399'999'999'999 for nanos), not epoch offsets, so the semantics mirror the
// time32 (millis-of-day) functions in time.cc.

#include "./types.h"

extern "C" {

#define EXTRACT_TIME64(SUFFIX, UNITS_PER_SECOND)          \
  FORCE_INLINE                                            \
  gdv_int64 extractHour_time64##SUFFIX(gdv_time64 in) {   \
    return in / ((UNITS_PER_SECOND)*3600LL);              \
  }                                                       \
  FORCE_INLINE                                            \
  gdv_int64 extractMinute_time64##SUFFIX(gdv_time64 in) { \
    return (in / ((UNITS_PER_SECOND)*60LL)) % 60;         \
  }                                                       \
  FORCE_INLINE                                            \
  gdv_int64 extractSecond_time64##SUFFIX(gdv_time64 in) { \
    return (in / (UNITS_PER_SECOND)) % 60;                \
  }

// MICROSECOND (registered unit)
EXTRACT_TIME64(, 1000000LL)

// NANOSECOND
EXTRACT_TIME64(_ns, 1000000000LL)

#undef EXTRACT_TIME64

}  // extern "C"
