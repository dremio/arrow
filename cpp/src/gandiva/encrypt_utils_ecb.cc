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

#include "gandiva/encrypt_utils_ecb.h"
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stdexcept>
#include <cstring>
#include <sstream>

namespace gandiva {

namespace {

std::string get_openssl_error_string() {
  unsigned long error_code = ERR_get_error();
  if (error_code == 0) {
    return "Unknown OpenSSL error";
  }
  return std::string(ERR_reason_error_string(error_code));
}

const EVP_CIPHER* get_ecb_cipher_algo(int32_t key_length) {
  switch (key_length) {
    case 16:
      return EVP_aes_128_ecb();
    case 24:
      return EVP_aes_192_ecb();
    case 32:
      return EVP_aes_256_ecb();
    default: {
      std::ostringstream oss;
      oss << "Unsupported key length for AES-ECB: " << key_length
          << " bytes. Supported lengths: 16, 24, 32 bytes";
      throw std::runtime_error(oss.str());
    }
  }
}

}  // namespace

GANDIVA_EXPORT
int32_t aes_encrypt_ecb(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, unsigned char* cipher) {
  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_ecb_cipher_algo(key_len);

  if (!en_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_EncryptInit_ex(en_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key), nullptr)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not initialize EVP cipher context for encryption: " +
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
int32_t aes_decrypt_ecb(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, unsigned char* plaintext) {
  int32_t plaintext_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* de_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_ecb_cipher_algo(key_len);

  if (!de_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  if (!EVP_DecryptInit_ex(de_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key), nullptr)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not initialize EVP cipher context for decryption: " +
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

