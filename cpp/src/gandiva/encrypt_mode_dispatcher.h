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
   * @param plaintext The data to encrypt
   * @param plaintext_len Length of plaintext in bytes
   * @param key The encryption key (16, 24, or 32 bytes for AES-128/192/256)
   * @param key_len Length of key in bytes
   * @param key_validity Whether key is valid
   * @param mode Mode string (case-insensitive)
   * @param mode_len Length of mode string in bytes
   * @param mode_validity Whether mode is valid
   * @param iv The initialization vector
   * @param iv_len Length of the IV in bytes
   * @param iv_validity Whether IV is valid
   * @param fifth_argument Additional parameter (e.g. AAD for the GCM mode)
   * @param fifth_argument_len Length of fifth_argument in bytes
   * @param fifth_argument_validity Whether fifth_argument is valid
   * @param cipher Output buffer for encrypted data
   * @return Length of encrypted data in bytes
   * @throws std::runtime_error on encryption failure, unsupported mode, or invalid parameters
   */
  static int32_t encrypt(const char* plaintext, int32_t plaintext_len,
                         const char* key, int32_t key_len, bool key_validity,
                         const char* mode, int32_t mode_len, bool mode_validity,
                         const char* iv, int32_t iv_len, bool iv_validity,
                         const char* fifth_argument, int32_t fifth_argument_len,
                         bool fifth_argument_validity,
                         unsigned char* cipher);

  /**
   * Decrypt data using the specified mode
   *
   * @param ciphertext The data to decrypt (format depends on mode and IV parameter)
   * @param ciphertext_len Length of ciphertext in bytes
   * @param key The decryption key (16, 24, or 32 bytes for AES-128/192/256)
   * @param key_len Length of key in bytes
   * @param key_validity Whether key is valid
   * @param mode Mode string (case-insensitive)
   * @param mode_len Length of mode string in bytes
   * @param mode_validity Whether mode is valid
   * @param iv The initialization vector
   * @param iv_len Length of the IV in bytes
   * @param iv_validity Whether IV is valid
   * @param fifth_argument Additional parameter (e.g. AAD for the GCM mode)
   * @param fifth_argument_len Length of fifth_argument in bytes
   * @param fifth_argument_validity Whether fifth_argument is valid
   * @param plaintext Output buffer for decrypted data
   * @return Length of decrypted data in bytes (plaintext only, IV and tag removed)
   * @throws std::runtime_error on decryption failure, unsupported mode, invalid parameters, or authentication failure
   */
  static int32_t decrypt(const char* ciphertext, int32_t ciphertext_len,
                         const char* key, int32_t key_len, bool key_validity,
                         const char* mode, int32_t mode_len, bool mode_validity,
                         const char* iv, int32_t iv_len, bool iv_validity,
                         const char* fifth_argument, int32_t fifth_argument_len,
                         bool fifth_argument_validity,
                         unsigned char* plaintext);
};

}  // namespace gandiva

#endif  // GANDIVA_ENCRYPT_MODE_DISPATCHER_H

