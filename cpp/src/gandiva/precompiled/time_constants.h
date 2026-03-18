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

// Millisecond-based constants (existing)
#define MILLIS_IN_SEC (1000LL)
#define MILLIS_IN_MIN (60LL * MILLIS_IN_SEC)
#define MILLIS_IN_HOUR (60LL * MILLIS_IN_MIN)
#define MILLIS_IN_DAY (24LL * MILLIS_IN_HOUR)
#define MILLIS_IN_WEEK (7LL * MILLIS_IN_DAY)

// Microsecond-based constants
#define MICROS_IN_MILLI (1000LL)
#define MICROS_IN_SEC (1000000LL)
#define MICROS_IN_MIN (60LL * MICROS_IN_SEC)
#define MICROS_IN_HOUR (60LL * MICROS_IN_MIN)
#define MICROS_IN_DAY (24LL * MICROS_IN_HOUR)
#define MICROS_IN_WEEK (7LL * MICROS_IN_DAY)

// Nanosecond-based constants
#define NANOS_IN_MICRO (1000LL)
#define NANOS_IN_MILLI (1000000LL)
#define NANOS_IN_SEC (1000000000LL)
#define NANOS_IN_MIN (60LL * NANOS_IN_SEC)
#define NANOS_IN_HOUR (60LL * NANOS_IN_MIN)
#define NANOS_IN_DAY (24LL * NANOS_IN_HOUR)
#define NANOS_IN_WEEK (7LL * NANOS_IN_DAY)

// Seconds-based constants
#define SECS_IN_MIN (60LL)
#define SECS_IN_HOUR (60LL * SECS_IN_MIN)
#define SECS_IN_DAY (24LL * SECS_IN_HOUR)
#define SECS_IN_WEEK (7LL * SECS_IN_DAY)

// Millisecond conversion macros (existing)
#define MILLIS_TO_SEC(millis) ((millis) / MILLIS_IN_SEC)
#define MILLIS_TO_MINS(millis) ((millis) / MILLIS_IN_MIN)
#define MILLIS_TO_HOUR(millis) ((millis) / MILLIS_IN_HOUR)
#define MILLIS_TO_DAY(millis) ((millis) / MILLIS_IN_DAY)
#define MILLIS_TO_WEEK(millis) ((millis) / MILLIS_IN_WEEK)

// Microsecond conversion macros
#define MICROS_TO_SEC(micros) ((micros) / MICROS_IN_SEC)
#define MICROS_TO_MINS(micros) ((micros) / MICROS_IN_MIN)
#define MICROS_TO_HOUR(micros) ((micros) / MICROS_IN_HOUR)
#define MICROS_TO_DAY(micros) ((micros) / MICROS_IN_DAY)
#define MICROS_TO_MILLIS(micros) ((micros) / MICROS_IN_MILLI)

// Nanosecond conversion macros
#define NANOS_TO_SEC(nanos) ((nanos) / NANOS_IN_SEC)
#define NANOS_TO_MINS(nanos) ((nanos) / NANOS_IN_MIN)
#define NANOS_TO_HOUR(nanos) ((nanos) / NANOS_IN_HOUR)
#define NANOS_TO_DAY(nanos) ((nanos) / NANOS_IN_DAY)
#define NANOS_TO_MILLIS(nanos) ((nanos) / NANOS_IN_MILLI)
#define NANOS_TO_MICROS(nanos) ((nanos) / NANOS_IN_MICRO)

// Upward conversion macros
#define SECS_TO_MILLIS(secs) ((secs) * MILLIS_IN_SEC)
#define SECS_TO_MICROS(secs) ((secs) * MICROS_IN_SEC)
#define SECS_TO_NANOS(secs) ((secs) * NANOS_IN_SEC)
#define MILLIS_TO_MICROS(millis) ((millis) * MICROS_IN_MILLI)
#define MILLIS_TO_NANOS(millis) ((millis) * NANOS_IN_MILLI)
#define MICROS_TO_NANOS(micros) ((micros) * NANOS_IN_MICRO)
