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

#include "gandiva/encrypt_utils.h"
#include <algorithm>
#include <stdexcept>

namespace gandiva {

int32_t aes_encrypt(const char* plaintext, int32_t plaintext_len, const char* key,
                    int32_t key_len, const std::string& mode, const char* iv,
                    int32_t iv_len, bool use_padding, unsigned char* cipher) {
  std::string mode_upper = mode;
  std::transform(mode_upper.begin(), mode_upper.end(), mode_upper.begin(), ::toupper);

  if (mode_upper == "ECB") {
    if (iv_len != 0) {
      throw std::runtime_error("ECB mode does not use IV");
    }
    return aes_encrypt_ecb(plaintext, plaintext_len, key, key_len, cipher);
  } else if (mode_upper == "CBC") {
    return aes_encrypt_cbc(plaintext, plaintext_len, key, key_len, iv, iv_len, cipher,
                           use_padding);
  } else if (mode_upper == "GCM") {
    throw std::runtime_error("GCM mode not yet implemented");
  } else {
    throw std::runtime_error("Unsupported encryption mode: " + mode);
  }
}

int32_t aes_decrypt(const char* ciphertext, int32_t ciphertext_len, const char* key,
                    int32_t key_len, const std::string& mode, const char* iv,
                    int32_t iv_len, bool use_padding, unsigned char* plaintext) {
  std::string mode_upper = mode;
  std::transform(mode_upper.begin(), mode_upper.end(), mode_upper.begin(), ::toupper);

  if (mode_upper == "ECB") {
    if (iv_len != 0) {
      throw std::runtime_error("ECB mode does not use IV");
    }
    return aes_decrypt_ecb(ciphertext, ciphertext_len, key, key_len, plaintext);
  } else if (mode_upper == "CBC") {
    return aes_decrypt_cbc(ciphertext, ciphertext_len, key, key_len, iv, iv_len, plaintext,
                           use_padding);
  } else if (mode_upper == "GCM") {
    throw std::runtime_error("GCM mode not yet implemented");
  } else {
    throw std::runtime_error("Unsupported decryption mode: " + mode);
  }
}

}  // namespace gandiva

