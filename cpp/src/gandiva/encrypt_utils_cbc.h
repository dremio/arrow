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

#include <cstdint>
#include <openssl/evp.h>
#include "gandiva/visibility.h"

namespace gandiva {

// CBC mode identifiers
constexpr const char* AES_CBC_MODE = "AES-CBC";
constexpr const char* AES_CBC_PKCS7_MODE = "AES-CBC-PKCS7";
constexpr const char* AES_CBC_NONE_MODE = "AES-CBC-NONE";

// CBC IV length in bytes
constexpr int32_t CBC_IV_LENGTH = 16;  // 16 bytes (128 bits) - required for CBC

/**
 * Encrypt data using AES-CBC algorithm with explicit padding mode
 *
 * Output format:
 * - With NULL IV (auto-generated): [16-byte IV][ciphertext]
 * - With user-supplied IV: [ciphertext]
 *
 * IV Handling:
 * - If iv is NULL or iv_len is 0: A cryptographically secure random 16-byte IV
 *   is automatically generated using OpenSSL RAND_bytes and prepended to output
 * - If iv is provided: It must be exactly 16 bytes, will be used as-is, and NOT prepended
 *
 * @param plaintext The data to encrypt
 * @param plaintext_len Length of plaintext in bytes
 * @param key The encryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (NULL for auto-generation, or exactly 16 bytes)
 * @param iv_len Length of IV in bytes (0 for auto-generation, or 16)
 * @param use_padding Whether to use PKCS7 padding (true) or no padding (false)
 * @param cipher Output buffer for encrypted data (must be at least plaintext_len + 32 bytes)
 * @return Length of encrypted data in bytes (16 + ciphertext_len)
 * @throws std::runtime_error on encryption failure or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_encrypt_cbc(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        bool use_padding, unsigned char* cipher);

/**
 * Decrypt data using AES-CBC algorithm with explicit padding mode
 *
 * IV Handling:
 * - If iv is NULL or iv_len is 0: IV is extracted from the first 16 bytes of ciphertext
 *   (expects format: [16-byte IV][ciphertext])
 * - If iv is provided: It must be exactly 16 bytes, and ciphertext should be
 *   [ciphertext] without embedded IV
 *
 * @param ciphertext The data to decrypt
 *   - With NULL IV: [16-byte IV][ciphertext] (min 32 bytes)
 *   - With provided IV: [ciphertext]
 * @param ciphertext_len Length of ciphertext in bytes (includes IV if embedded)
 * @param key The decryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (NULL for extraction, or exactly 16 bytes)
 * @param iv_len Length of IV in bytes (0 for extraction, or 16)
 * @param use_padding Whether to use PKCS7 padding (true) or no padding (false)
 * @param plaintext Output buffer for decrypted data
 * @return Length of decrypted data in bytes
 * @throws std::runtime_error on decryption failure or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_decrypt_cbc(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        bool use_padding, unsigned char* plaintext);

}  // namespace gandiva

