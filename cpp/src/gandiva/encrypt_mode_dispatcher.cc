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

#include "gandiva/encrypt_mode_dispatcher.h"
#include "gandiva/encrypt_utils_ecb.h"
#include "gandiva/encrypt_utils_cbc.h"
#include "arrow/util/string.h"
#include <string>
#include <sstream>
#include <stdexcept>

namespace gandiva {

int32_t EncryptModeDispatcher::encrypt(
    const char* plaintext, int32_t plaintext_len, const char* key,
    int32_t key_len, const char* mode, int32_t mode_len, const char* iv,
    int32_t iv_len, const char* fifth_argument, int32_t fifth_argument_len,
    unsigned char* cipher) {
  std::string mode_str =
      arrow::internal::AsciiToUpper(std::string_view(mode, mode_len));

  if (mode_str == "AES-ECB") {
    return aes_encrypt_ecb(plaintext, plaintext_len, key, key_len, cipher);
  } else if (mode_str == "AES-CBC-PKCS7") {
    return aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                           iv, iv_len, true, cipher);
  } else if (mode_str == "AES-CBC-NONE") {
    return aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                           iv, iv_len, false, cipher);
  } else if (mode_str == "AES-GCM") {
    throw std::runtime_error("AES-GCM encryption mode is not yet implemented");
  } else {
    std::ostringstream oss;
    oss << "Unsupported encryption mode: " << mode_str
        << ". Supported modes: AES-ECB, AES-CBC-PKCS7, AES-CBC-NONE";
    throw std::runtime_error(oss.str());
  }
}

int32_t EncryptModeDispatcher::decrypt(
    const char* ciphertext, int32_t ciphertext_len, const char* key,
    int32_t key_len, const char* mode, int32_t mode_len, const char* iv,
    int32_t iv_len, const char* fifth_argument, int32_t fifth_argument_len,
    unsigned char* plaintext) {
  std::string mode_str =
      arrow::internal::AsciiToUpper(std::string_view(mode, mode_len));

  if (mode_str == "AES-ECB") {
    return aes_decrypt_ecb(ciphertext, ciphertext_len, key, key_len, plaintext);
  } else if (mode_str == "AES-CBC-PKCS7") {
    return aes_decrypt_cbc(ciphertext, ciphertext_len, key, key_len,
                           iv, iv_len, true, plaintext);
  } else if (mode_str == "AES-CBC-NONE") {
    return aes_decrypt_cbc(ciphertext, ciphertext_len, key, key_len,
                           iv, iv_len, false, plaintext);
  } else if (mode_str == "AES-GCM") {
    throw std::runtime_error("AES-GCM decryption mode is not yet implemented");
  } else {
    std::ostringstream oss;
    oss << "Unsupported decryption mode: " << mode_str
        << ". Supported modes: AES-ECB, AES-CBC-PKCS7, AES-CBC-NONE";
    throw std::runtime_error(oss.str());
  }
}

}  // namespace gandiva

