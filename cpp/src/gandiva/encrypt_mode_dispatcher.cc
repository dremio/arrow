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
#include "gandiva/encrypt_utils_gcm.h"
#include "arrow/util/string.h"
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace gandiva {

static const std::vector<std::string_view> SUPPORTED_MODES = {
    AES_ECB_MODE, AES_ECB_PKCS7_MODE, AES_ECB_NONE_MODE,
    AES_CBC_MODE, AES_CBC_PKCS7_MODE, AES_CBC_NONE_MODE,
    AES_GCM_MODE
};

enum class EncryptionMode {
  ECB,
  ECB_PKCS7,
  ECB_NONE,
  CBC,
  CBC_PKCS7,
  CBC_NONE,
  GCM,
  NULL_VALUE,
  UNKNOWN
};

EncryptionMode ParseEncryptionMode(const char* mode, int32_t mode_len, bool mode_validity) {
  if (!mode_validity) {
    return EncryptionMode::NULL_VALUE;
  }

  // Convert mode string to uppercase for case-insensitive comparison
  std::string mode_str =
      arrow::internal::AsciiToUpper(std::string_view(mode, mode_len));

  if (mode_str == AES_ECB_MODE) return EncryptionMode::ECB;
  if (mode_str == AES_ECB_PKCS7_MODE) return EncryptionMode::ECB_PKCS7;
  if (mode_str == AES_ECB_NONE_MODE) return EncryptionMode::ECB_NONE;
  if (mode_str == AES_CBC_MODE) return EncryptionMode::CBC;
  if (mode_str == AES_CBC_PKCS7_MODE) return EncryptionMode::CBC_PKCS7;
  if (mode_str == AES_CBC_NONE_MODE) return EncryptionMode::CBC_NONE;
  if (mode_str == AES_GCM_MODE) return EncryptionMode::GCM;

  return EncryptionMode::UNKNOWN;
}

std::string BuildUnsupportedModeError(const char* operation, const char* mode, int32_t mode_len) {
  std::string modes_str = arrow::internal::JoinStrings(SUPPORTED_MODES, ", ");
  std::ostringstream oss;
  oss << "Unsupported " << operation << " mode: " << std::string_view(mode, mode_len)
      << ". Supported modes: " << modes_str;
  return oss.str();
}

int32_t EncryptModeDispatcher::encrypt(
    const char* plaintext, int32_t plaintext_len,
    const char* key, int32_t key_len, bool key_validity,
    const char* mode, int32_t mode_len, bool mode_validity,
    const char* iv, int32_t iv_len, bool iv_validity,
    const char* fifth_argument, int32_t fifth_argument_len,
    bool fifth_argument_validity, unsigned char* cipher) {
  if (!key_validity) {
    throw std::runtime_error("Encryption key cannot be NULL");
  }

  // Handle NULL IV: pass nullptr and 0 to trigger auto-generation
  const char* actual_iv = iv_validity ? iv : nullptr;
  int32_t actual_iv_len = iv_validity ? iv_len : 0;

  // Handle NULL fifth_argument (e.g., AAD for GCM)
  const char* actual_fifth_arg = fifth_argument_validity ? fifth_argument : nullptr;
  int32_t actual_fifth_arg_len = fifth_argument_validity ? fifth_argument_len : 0;

  switch (ParseEncryptionMode(mode, mode_len, mode_validity)) {
    case EncryptionMode::ECB:
    case EncryptionMode::ECB_PKCS7:
      return aes_encrypt_ecb(plaintext, plaintext_len, key, key_len, true, cipher);
    case EncryptionMode::ECB_NONE:
      return aes_encrypt_ecb(plaintext, plaintext_len, key, key_len, false, cipher);
    case EncryptionMode::CBC:
    case EncryptionMode::CBC_PKCS7:
      return aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                             actual_iv, actual_iv_len, true, cipher);
    case EncryptionMode::CBC_NONE:
      return aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                             actual_iv, actual_iv_len, false, cipher);
    case EncryptionMode::GCM:
      return aes_encrypt_gcm(plaintext, plaintext_len, key, key_len,
                             actual_iv, actual_iv_len, actual_fifth_arg, actual_fifth_arg_len, cipher);
    case EncryptionMode::NULL_VALUE:
      throw std::runtime_error(BuildUnsupportedModeError("encryption", "NULL", 4));
    case EncryptionMode::UNKNOWN:
    default:
      throw std::runtime_error(BuildUnsupportedModeError("encryption", mode, mode_len));
  }
}

int32_t EncryptModeDispatcher::decrypt(
    const char* ciphertext, int32_t ciphertext_len,
    const char* key, int32_t key_len, bool key_validity,
    const char* mode, int32_t mode_len, bool mode_validity,
    const char* iv, int32_t iv_len, bool iv_validity,
    const char* fifth_argument, int32_t fifth_argument_len,
    bool fifth_argument_validity, unsigned char* plaintext) {
  // If key is NULL (validity flag is false), throw error
  if (!key_validity) {
    throw std::runtime_error("Decryption key cannot be NULL");
  }

  // Handle NULL IV: pass nullptr and 0 to extract IV from ciphertext
  const char* actual_iv = iv_validity ? iv : nullptr;
  int32_t actual_iv_len = iv_validity ? iv_len : 0;

  // Handle NULL fifth_argument (e.g., AAD for GCM)
  const char* actual_fifth_arg = fifth_argument_validity ? fifth_argument : nullptr;
  int32_t actual_fifth_arg_len = fifth_argument_validity ? fifth_argument_len : 0;

  switch (ParseEncryptionMode(mode, mode_len, mode_validity)) {
    case EncryptionMode::ECB:
    case EncryptionMode::ECB_PKCS7:
      return aes_decrypt_ecb(ciphertext, ciphertext_len, key, key_len, true, plaintext);
    case EncryptionMode::ECB_NONE:
      return aes_decrypt_ecb(ciphertext, ciphertext_len, key, key_len, false, plaintext);
    case EncryptionMode::CBC:
    case EncryptionMode::CBC_PKCS7:
      return aes_decrypt_cbc(ciphertext, ciphertext_len, key, key_len,
                             actual_iv, actual_iv_len, true, plaintext);
    case EncryptionMode::CBC_NONE:
      // CBC mode without padding
      return aes_decrypt_cbc(ciphertext, ciphertext_len, key, key_len,
                             actual_iv, actual_iv_len, false, plaintext);
    case EncryptionMode::GCM:
      return aes_decrypt_gcm(ciphertext, ciphertext_len, key, key_len,
                             actual_iv, actual_iv_len, actual_fifth_arg, actual_fifth_arg_len, plaintext);
    case EncryptionMode::UNKNOWN:
    default:
      if (!mode_validity) {
        throw std::runtime_error(BuildUnsupportedModeError("decryption", "NULL", 4));
      }
      throw std::runtime_error(BuildUnsupportedModeError("decryption", mode, mode_len));
  }
}

}  // namespace gandiva

