// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License") you may not use this file except in compliance
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

#ifndef GANDIVA_ENCRYPT_MODE_DISPATCHER_H
#define GANDIVA_ENCRYPT_MODE_DISPATCHER_H

#include <cstdint>

namespace gandiva {

/**
 * Dispatcher for AES encryption/decryption based on mode string.
 * Routes calls to appropriate implementation.
 */
class EncryptModeDispatcher {
 public:
  /**
   * Encrypt data using the specified mode
   *
   * Supported modes:
   * - AES-ECB, AES-ECB-PKCS7, AES-ECB-NONE: ECB mode (no IV)
   * - AES-CBC, AES-CBC-PKCS7, AES-CBC-NONE: CBC mode (requires 16-byte IV)
   * - AES-GCM: GCM mode (requires 12-byte IV)
   *
   * Output format:
   * - ECB: [ciphertext]
   * - CBC with auto-generated IV: [16-byte IV][ciphertext]
   * - CBC with user-supplied IV: [ciphertext]
   * - GCM with auto-generated IV: [12-byte IV][ciphertext][16-byte authentication tag]
   * - GCM with user-supplied IV: [ciphertext][16-byte authentication tag]
   *
   * IV Handling (CBC and GCM modes):
   * - If iv is NULL or iv_len is 0: A cryptographically secure random IV is
   *   automatically generated and prepended to the output
   * - If iv is provided: It must be the exact required length (12 for GCM, 16 for CBC),
   *   and will NOT be prepended to the output (only ciphertext is returned)
   *
   * @param plaintext The data to encrypt
   * @param plaintext_len Length of plaintext in bytes
   * @param key The encryption key (16, 24, or 32 bytes for AES-128/192/256)
   * @param key_len Length of key in bytes
   * @param mode Mode string (case-insensitive)
   * @param mode_len Length of mode string in bytes
   * @param iv The initialization vector (NULL for auto-generation in CBC/GCM, ignored for ECB)
   * @param iv_len Length of the IV in bytes (0 for auto-generation, 12 for GCM, 16 for CBC)
   * @param fifth_argument Additional parameter (AAD for GCM mode, ignored for others)
   * @param fifth_argument_len Length of fifth_argument in bytes
   * @param cipher Output buffer for encrypted data (must be large enough for output format)
   * @return Length of encrypted data in bytes (includes prepended IV only if auto-generated)
   * @throws std::runtime_error on encryption failure, unsupported mode, or invalid parameters
   */
  static int32_t encrypt(const char* plaintext, int32_t plaintext_len,
                         const char* key, int32_t key_len,
                         const char* mode, int32_t mode_len,
                         const char* iv, int32_t iv_len,
                         const char* fifth_argument, int32_t fifth_argument_len,
                         unsigned char* cipher);

  /**
   * Decrypt data using the specified mode
   *
   * Supported modes:
   * - AES-ECB, AES-ECB-PKCS7, AES-ECB-NONE: ECB mode (no IV)
   * - AES-CBC, AES-CBC-PKCS7, AES-CBC-NONE: CBC mode (requires 16-byte IV)
   * - AES-GCM: GCM mode (requires 12-byte IV)
   *
   * Expected input format:
   * - ECB: [ciphertext]
   * - CBC with NULL IV: [16-byte IV][ciphertext]
   * - CBC with provided IV: [ciphertext] (IV provided separately)
   * - GCM with NULL IV: [12-byte IV][ciphertext][16-byte authentication tag]
   * - GCM with provided IV: [ciphertext][16-byte authentication tag] (IV provided separately)
   *
   * IV Handling (CBC and GCM modes):
   * - If iv is NULL or iv_len is 0: IV is extracted from the beginning of ciphertext
   * - If iv is provided: It must be the exact required length (12 for GCM, 16 for CBC),
   *   and ciphertext should not include the IV
   *
   * @param ciphertext The data to decrypt (format depends on mode and IV parameter)
   * @param ciphertext_len Length of ciphertext in bytes (includes IV if embedded)
   * @param key The decryption key (16, 24, or 32 bytes for AES-128/192/256)
   * @param key_len Length of key in bytes
   * @param mode Mode string (case-insensitive)
   * @param mode_len Length of mode string in bytes
   * @param iv The initialization vector (NULL for extraction from ciphertext, ignored for ECB)
   * @param iv_len Length of the IV in bytes (0 for extraction, 12 for GCM, 16 for CBC)
   * @param fifth_argument Additional parameter (AAD for GCM mode, ignored for others)
   * @param fifth_argument_len Length of fifth_argument in bytes
   * @param plaintext Output buffer for decrypted data
   * @return Length of decrypted data in bytes (plaintext only, IV and tag removed)
   * @throws std::runtime_error on decryption failure, unsupported mode, invalid parameters, or authentication failure
   */
  static int32_t decrypt(const char* ciphertext, int32_t ciphertext_len,
                         const char* key, int32_t key_len,
                         const char* mode, int32_t mode_len,
                         const char* iv, int32_t iv_len,
                         const char* fifth_argument, int32_t fifth_argument_len,
                         unsigned char* plaintext);
};

}  // namespace gandiva

#endif  // GANDIVA_ENCRYPT_MODE_DISPATCHER_H

