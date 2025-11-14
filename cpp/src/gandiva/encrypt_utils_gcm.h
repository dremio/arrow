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

/**
 * Encrypt data using AES-GCM algorithm without AAD
 *
 * @param plaintext The data to encrypt
 * @param plaintext_len Length of plaintext in bytes
 * @param key The encryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (must be exactly 12 bytes for GCM)
 * @param iv_len Length of IV in bytes (must be 12)
 * @param cipher Output buffer for encrypted data
 * @param tag Output buffer for authentication tag (typically 16 bytes)
 * @param tag_len Length of tag in bytes (4-16, typically 16)
 * @return Length of encrypted data in bytes
 * @throws std::runtime_error on encryption failure or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_encrypt_gcm(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        unsigned char* cipher, unsigned char* tag, int32_t tag_len);

/**
 * Encrypt data using AES-GCM algorithm with AAD
 *
 * @param plaintext The data to encrypt
 * @param plaintext_len Length of plaintext in bytes
 * @param key The encryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (must be exactly 12 bytes for GCM)
 * @param iv_len Length of IV in bytes (must be 12)
 * @param aad Additional authenticated data
 * @param aad_len Length of AAD in bytes
 * @param cipher Output buffer for encrypted data
 * @param tag Output buffer for authentication tag (typically 16 bytes)
 * @param tag_len Length of tag in bytes (4-16, typically 16)
 * @return Length of encrypted data in bytes
 * @throws std::runtime_error on encryption failure or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_encrypt_gcm_with_aad(const char* plaintext, int32_t plaintext_len,
                                  const char* key, int32_t key_len, const char* iv,
                                  int32_t iv_len, const char* aad, int32_t aad_len,
                                  unsigned char* cipher, unsigned char* tag,
                                  int32_t tag_len);

/**
 * Decrypt data using AES-GCM algorithm without AAD
 *
 * @param ciphertext The data to decrypt
 * @param ciphertext_len Length of ciphertext in bytes
 * @param key The decryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (must be exactly 12 bytes for GCM)
 * @param iv_len Length of IV in bytes (must be 12)
 * @param tag The authentication tag to verify
 * @param tag_len Length of tag in bytes (4-16, typically 16)
 * @param plaintext Output buffer for decrypted data
 * @return Length of decrypted data in bytes
 * @throws std::runtime_error on decryption failure, tag verification failure, or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_decrypt_gcm(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len, const char* tag,
                        int32_t tag_len, unsigned char* plaintext);

/**
 * Decrypt data using AES-GCM algorithm with AAD
 *
 * @param ciphertext The data to decrypt
 * @param ciphertext_len Length of ciphertext in bytes
 * @param key The decryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (must be exactly 12 bytes for GCM)
 * @param iv_len Length of IV in bytes (must be 12)
 * @param aad Additional authenticated data
 * @param aad_len Length of AAD in bytes
 * @param tag The authentication tag to verify
 * @param tag_len Length of tag in bytes (4-16, typically 16)
 * @param plaintext Output buffer for decrypted data
 * @return Length of decrypted data in bytes
 * @throws std::runtime_error on decryption failure, tag verification failure, or invalid parameters
 */
GANDIVA_EXPORT
int32_t aes_decrypt_gcm_with_aad(const char* ciphertext, int32_t ciphertext_len,
                                  const char* key, int32_t key_len, const char* iv,
                                  int32_t iv_len, const char* aad, int32_t aad_len,
                                  const char* tag, int32_t tag_len, unsigned char* plaintext);

}  // namespace gandiva

