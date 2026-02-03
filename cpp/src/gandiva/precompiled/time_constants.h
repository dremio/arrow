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

#define MILLIS_IN_SEC (1000LL)
#define MILLIS_IN_MIN (60 * MILLIS_IN_SEC)
#define MILLIS_IN_HOUR (60 * MILLIS_IN_MIN)
#define MILLIS_IN_DAY (24 * MILLIS_IN_HOUR)
#define MILLIS_IN_WEEK (7 * MILLIS_IN_DAY)

#define MILLIS_TO_SEC(millis) ((millis) / MILLIS_IN_SEC)
#define MILLIS_TO_MINS(millis) ((millis) / MILLIS_IN_MIN)
#define MILLIS_TO_HOUR(millis) ((millis) / MILLIS_IN_HOUR)
#define MILLIS_TO_DAY(millis) ((millis) / MILLIS_IN_DAY)
#define MILLIS_TO_WEEK(millis) ((millis) / MILLIS_IN_WEEK)

#define MICROS_IN_MILLIS (1000LL)
#define MICROS_IN_SEC (MICROS_IN_MILLIS * MILLIS_IN_SEC)
#define MICROS_IN_MIN (MICROS_IN_MILLIS * MILLIS_IN_MIN)
#define MICROS_IN_HOUR (MICROS_IN_MILLIS * MILLIS_IN_HOUR)
#define MICROS_IN_DAY (MICROS_IN_MILLIS * MILLIS_IN_DAY)
#define MICROS_IN_WEEK (MICROS_IN_MILLIS * MILLIS_IN_WEEK)

#define NANOS_IN_MICROS (1000LL)
#define NANOS_IN_MILLIS (NANOS_IN_MICROS * MICROS_IN_MILLIS)
#define NANOS_IN_SEC (NANOS_IN_MICROS * MICROS_IN_SEC)
#define NANOS_IN_MIN (NANOS_IN_MICROS * MICROS_IN_MIN)
#define NANOS_IN_HOUR (NANOS_IN_MICROS * MICROS_IN_HOUR)
#define NANOS_IN_DAY (NANOS_IN_MICROS * MICROS_IN_DAY)
#define NANOS_IN_WEEK (NANOS_IN_MICROS * MICROS_IN_WEEK)
