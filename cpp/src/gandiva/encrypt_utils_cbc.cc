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

#include "gandiva/encrypt_utils_cbc.h"
#include "gandiva/encrypt_utils_common.h"
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <cctype>

namespace gandiva {

namespace {

// Padding mode enum
enum class PaddingMode {
  PKCS7,
  NONE
};

const EVP_CIPHER* get_cbc_cipher_algo(int32_t key_length) {
  switch (key_length) {
    case 16:
      return EVP_aes_128_cbc();
    case 24:
      return EVP_aes_192_cbc();
    case 32:
      return EVP_aes_256_cbc();
    default: {
      std::ostringstream oss;
      oss << "Unsupported key length for AES-CBC: " << key_length
          << " bytes. Supported lengths: 16, 24, 32 bytes";
      throw std::runtime_error(oss.str());
    }
  }
}

PaddingMode get_padding_mode(const char* padding_str, int32_t padding_len) {
  if (padding_str == nullptr || padding_len <= 0) {
    throw std::runtime_error("Invalid padding parameter: null or empty");
  }

  // Case-insensitive comparison using strncasecmp
  if (strncasecmp(padding_str, "PKCS7", padding_len) == 0 && padding_len == 5) {
    return PaddingMode::PKCS7;
  } else if (strncasecmp(padding_str, "NONE", padding_len) == 0 && padding_len == 4) {
    return PaddingMode::NONE;
  } else {
    std::ostringstream oss;
    oss << "Invalid padding mode: '" << std::string(padding_str, padding_len)
        << "'. Supported modes: PKCS7, NONE (case-insensitive)";
    throw std::runtime_error(oss.str());
  }
}

}  // namespace

GANDIVA_EXPORT
int32_t aes_encrypt_cbc(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        const char* padding, int32_t padding_len, unsigned char* cipher) {
  // Validate IV length
  if (iv_len != 16) {
    std::ostringstream oss;
    oss << "Invalid IV length for AES-CBC: " << iv_len
        << " bytes. IV must be exactly 16 bytes";
    throw std::runtime_error(oss.str());
  }

  PaddingMode padding_mode = get_padding_mode(padding, padding_len);

  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_cbc_cipher_algo(key_len);

  if (!en_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_EncryptInit_ex(en_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key),
                          reinterpret_cast<const unsigned char*>(iv))) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not initialize EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  int padding_flag = (padding_mode == PaddingMode::PKCS7) ? 1 : 0;
  if (!EVP_CIPHER_CTX_set_padding(en_ctx, padding_flag)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not set padding mode for encryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_EncryptUpdate(en_ctx, cipher, &len,
                         reinterpret_cast<const unsigned char*>(plaintext),
                         plaintext_len)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not update EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  cipher_len += len;

  if (!EVP_EncryptFinal_ex(en_ctx, cipher + len, &len)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not finalize EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  cipher_len += len;

  EVP_CIPHER_CTX_free(en_ctx);
  return cipher_len;
}

GANDIVA_EXPORT
int32_t aes_decrypt_cbc(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        const char* padding, int32_t padding_len, unsigned char* plaintext) {
  // Validate IV length
  if (iv_len != 16) {
    std::ostringstream oss;
    oss << "Invalid IV length for AES-CBC: " << iv_len
        << " bytes. IV must be exactly 16 bytes";
    throw std::runtime_error(oss.str());
  }

  PaddingMode padding_mode = get_padding_mode(padding, padding_len);

  int32_t plaintext_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* de_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_cbc_cipher_algo(key_len);

  if (!de_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_DecryptInit_ex(de_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key),
                          reinterpret_cast<const unsigned char*>(iv))) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not initialize EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  int padding_flag = (padding_mode == PaddingMode::PKCS7) ? 1 : 0;
  if (!EVP_CIPHER_CTX_set_padding(de_ctx, padding_flag)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not set padding mode for decryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_DecryptUpdate(de_ctx, plaintext, &len,
                         reinterpret_cast<const unsigned char*>(ciphertext),
                         ciphertext_len)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not update EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  plaintext_len += len;

  if (!EVP_DecryptFinal_ex(de_ctx, plaintext + len, &len)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not finalize EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  plaintext_len += len;

  EVP_CIPHER_CTX_free(de_ctx);
  return plaintext_len;
}

}  // namespace gandiva

