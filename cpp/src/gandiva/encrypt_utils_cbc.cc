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
#include "gandiva/encrypt_utils_iv.h"
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <cctype>

namespace gandiva {

namespace {

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

void validate_iv_length_cbc(int32_t iv_len) {
  if (iv_len != CBC_IV_LENGTH) {
    std::ostringstream oss;
    oss << "Invalid IV length for AES-CBC: " << iv_len
        << " bytes. IV must be exactly " << CBC_IV_LENGTH << " bytes";
    throw std::runtime_error(oss.str());
  }
}

void validate_ciphertext_with_embedded_iv_cbc(int32_t ciphertext_len) {
  constexpr int32_t MIN_CIPHERTEXT_LEN = CBC_IV_LENGTH + 16;  // IV + minimum one block
  if (ciphertext_len < MIN_CIPHERTEXT_LEN) {
    std::ostringstream oss;
    oss << "Ciphertext too short for AES-CBC with embedded IV: " << ciphertext_len
        << " bytes. Must be at least " << MIN_CIPHERTEXT_LEN
        << " bytes (16-byte IV + minimum 16-byte block)";
    throw std::runtime_error(oss.str());
  }
}

}  // namespace

GANDIVA_EXPORT
int32_t aes_encrypt_cbc(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        bool use_padding, unsigned char* cipher) {
  // Buffer for IV (either user-supplied or auto-generated)
  unsigned char iv_buffer[CBC_IV_LENGTH];
  const unsigned char* actual_iv = nullptr;
  bool iv_auto_generated = false;

  if (iv == nullptr || iv_len == 0) {
    generate_random_iv(iv_buffer, CBC_IV_LENGTH);
    actual_iv = iv_buffer;
    iv_auto_generated = true;
  } else {
    validate_iv_length_cbc(iv_len);
    actual_iv = reinterpret_cast<const unsigned char*>(iv);
    iv_auto_generated = false;
  }

  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_cbc_cipher_algo(key_len);

  if (!en_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  // Only prepend IV to output if it was auto-generated
  // Auto-generated IV: [16-byte IV][ciphertext]
  // User-supplied IV: [ciphertext]
  if (iv_auto_generated) {
    std::memcpy(cipher, actual_iv, CBC_IV_LENGTH);
    cipher_len = CBC_IV_LENGTH;
  }

  if (!EVP_EncryptInit_ex(en_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key),
                          actual_iv)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not initialize EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  int padding_flag = use_padding ? 1 : 0;
  if (!EVP_CIPHER_CTX_set_padding(en_ctx, padding_flag)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not set padding mode for encryption: " +
                             get_openssl_error_string());
  }

  // Encrypt plaintext (write after IV)
  if (!EVP_EncryptUpdate(en_ctx, cipher + cipher_len, &len,
                         reinterpret_cast<const unsigned char*>(plaintext),
                         plaintext_len)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not update EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  cipher_len += len;

  if (!EVP_EncryptFinal_ex(en_ctx, cipher + cipher_len, &len)) {
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
                        bool use_padding, unsigned char* plaintext) {
  // Buffer for extracted IV (if needed)
  unsigned char iv_buffer[CBC_IV_LENGTH];
  const unsigned char* actual_iv = nullptr;
  const char* actual_ciphertext = ciphertext;
  int32_t actual_ciphertext_len = ciphertext_len;

  if (iv == nullptr) {
    validate_ciphertext_with_embedded_iv_cbc(ciphertext_len);
    extract_iv_from_ciphertext(ciphertext, ciphertext_len, CBC_IV_LENGTH,
                               iv_buffer, &actual_ciphertext,
                               &actual_ciphertext_len);
    actual_iv = iv_buffer;
  } else {
    validate_iv_length_cbc(iv_len);
    actual_iv = reinterpret_cast<const unsigned char*>(iv);
  }

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
                          actual_iv)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not initialize EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  int padding_flag = use_padding ? 1 : 0;
  if (!EVP_CIPHER_CTX_set_padding(de_ctx, padding_flag)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not set padding mode for decryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_DecryptUpdate(de_ctx, plaintext, &len,
                         reinterpret_cast<const unsigned char*>(actual_ciphertext),
                         actual_ciphertext_len)) {
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

