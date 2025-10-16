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
 * Get the EVP cipher algorithm for AES-CBC with the specified key length
 * Supports 128-bit (16 bytes), 192-bit (24 bytes), and 256-bit (32 bytes) keys
 *
 * @param key_length The length of the encryption key in bytes
 * @return The EVP_CIPHER pointer for AES-CBC
 * @throws std::runtime_error if key length is unsupported
 */
const EVP_CIPHER* get_cbc_cipher_algo(int32_t key_length);

/**
 * Encrypt data using AES-CBC algorithm with IV
 *
 * CBC (Cipher Block Chaining) mode provides semantic security by using an
 * initialization vector (IV). Each plaintext block is XORed with the previous
 * ciphertext block before encryption.
 *
 * @param plaintext The data to encrypt
 * @param plaintext_len Length of plaintext in bytes
 * @param key The encryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (must be 16 bytes for AES)
 * @param iv_len Length of IV in bytes (must be 16)
 * @param cipher Output buffer for encrypted data
 * @param use_padding If true, applies PKCS7 padding to plaintext; if false, plaintext must be
 *                    a multiple of 16 bytes (default: true)
 * @return Length of encrypted data in bytes
 * @throws std::runtime_error on encryption failure or invalid IV length
 */
GANDIVA_EXPORT
int32_t aes_encrypt_cbc(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        unsigned char* cipher, bool use_padding = true);

/**
 * Decrypt data using AES-CBC algorithm with IV
 *
 * @param ciphertext The data to decrypt
 * @param ciphertext_len Length of ciphertext in bytes
 * @param key The decryption key (16, 24, or 32 bytes for 128, 192, 256-bit keys)
 * @param key_len Length of key in bytes
 * @param iv The initialization vector (must be 16 bytes for AES)
 * @param iv_len Length of IV in bytes (must be 16)
 * @param plaintext Output buffer for decrypted data
 * @param use_padding If true, removes PKCS7 padding from decrypted data; if false, no padding
 *                    removal (plaintext must have been encrypted without padding) (default: true)
 * @return Length of decrypted data in bytes
 * @throws std::runtime_error on decryption failure or invalid IV length
 */
GANDIVA_EXPORT
int32_t aes_decrypt_cbc(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        unsigned char* plaintext, bool use_padding = true);

}  // namespace gandiva

