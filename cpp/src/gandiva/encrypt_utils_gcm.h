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

// GCM mode identifier
constexpr const char* AES_GCM_MODE = "AES-GCM";

// GCM IV length in bytes
constexpr int32_t GCM_IV_LENGTH = 12;  // 12 bytes (96 bits) - recommended for GCM but agreed to enforce it

// GCM authentication tag length in bytes
constexpr int32_t GCM_TAG_LENGTH = 16;

/**
 * Encrypt data using AES-GCM algorithm
 *
 * Output format:
 * - With NULL IV (auto-generated): [12-byte IV][ciphertext][16-byte authentication tag]
 * - With user-supplied IV: [ciphertext][16-byte authentication tag]
 *
 * IV Handling:
 * - If iv is NULL: A cryptographically secure random 12-byte IV
 *   is automatically generated using OpenSSL RAND_bytes and prepended to output
 * - If iv is provided: It must be exactly 12 bytes, will be used as-is, and not prepended
 *
 * @param plaintext The data to encrypt
 * @param plaintext_len Length of plaintext in bytes
 * @param key The encryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (NULL for auto-generation, or exactly 12 bytes)
 * @param iv_len Length of IV in bytes
 * @param aad Optional additional authenticated data (can be null)
 * @param aad_len Length of AAD in bytes
 * @param cipher Output buffer for encrypted data (must be at least plaintext_len + 28 bytes)
 * @return Length of encrypted data in bytes (12 + plaintext_len + 16)
 * @throws std::runtime_error on encryption failure or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_encrypt_gcm(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        const char* aad, int32_t aad_len, unsigned char* cipher);

/**
 * Decrypt data using AES-GCM algorithm
 *
 * IV Handling:
 * - If iv is NULL or iv_len is 0: IV is extracted from the first 12 bytes of ciphertext
 *   (expects format: [12-byte IV][ciphertext][16-byte tag])
 * - If iv is provided: It must be exactly 12 bytes, and ciphertext should be
 *   [ciphertext][16-byte tag] without embedded IV
 *
 * @param ciphertext The data to decrypt
 *   - With NULL IV: [12-byte IV][ciphertext][16-byte tag] (min 28 bytes)
 *   - With provided IV: [ciphertext][16-byte tag] (min 16 bytes)
 * @param ciphertext_len Length of ciphertext in bytes (includes IV if embedded, and tag)
 * @param key The decryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (NULL for extraction, or exactly 12 bytes)
 * @param iv_len Length of IV in bytes (0 for extraction, or 12)
 * @param aad Optional additional authenticated data (can be null)
 * @param aad_len Length of AAD in bytes (0 if aad is null)
 * @param plaintext Output buffer for decrypted data
 * @return Length of decrypted data in bytes
 * @throws std::runtime_error on decryption failure, invalid parameters, or tag verification failure
 */
GANDIVA_EXPORT
int32_t aes_decrypt_gcm(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        const char* aad, int32_t aad_len, unsigned char* plaintext);

}  // namespace gandiva

