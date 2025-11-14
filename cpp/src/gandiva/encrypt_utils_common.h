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

#ifndef GANDIVA_ENCRYPT_UTILS_COMMON_H
#define GANDIVA_ENCRYPT_UTILS_COMMON_H

#include <string>

namespace gandiva {

/// @brief Get a human-readable error string from OpenSSL's error queue.
/// @return A string describing the most recent OpenSSL error, or "Unknown OpenSSL error"
///         if no error is available.
std::string get_openssl_error_string();

}  // namespace gandiva

#endif  // GANDIVA_ENCRYPT_UTILS_COMMON_H

