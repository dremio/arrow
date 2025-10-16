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

#include <openssl/evp.h>
#include <cstdint>

namespace gandiva {

/**
 * Common utility functions for encryption operations
 */

/**
 * Get the appropriate EVP cipher algorithm based on key length
 * Supports 128-bit (16 bytes), 192-bit (24 bytes), and 256-bit (32 bytes) keys
 * 
 * @param key_length The length of the encryption key in bytes
 * @param cipher_type The type of cipher (e.g., "ecb", "cbc", "gcm")
 * @return The EVP_CIPHER pointer for the specified algorithm
 * @throws std::runtime_error if key length is unsupported
 */
const EVP_CIPHER* get_cipher_algo(int32_t key_length, const char* cipher_type = "ecb");

}  // namespace gandiva

