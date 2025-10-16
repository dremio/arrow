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
#include <openssl/aes.h>
#include <stdexcept>
#include <cstring>

namespace gandiva {

const EVP_CIPHER* get_cbc_cipher_algo(int32_t key_length) {
  switch (key_length) {
    case 16:
      return EVP_aes_128_cbc();
    case 24:
      return EVP_aes_192_cbc();
    case 32:
      return EVP_aes_256_cbc();
    default:
      throw std::runtime_error("unsupported key length for AES-CBC");
  }
}

GANDIVA_EXPORT
int32_t aes_encrypt_cbc(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        unsigned char* cipher, bool use_padding) {
  if (iv_len != 16) {
    throw std::runtime_error("IV length must be 16 bytes for AES-CBC");
  }

  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_cbc_cipher_algo(key_len);

  if (!en_ctx) {
    throw std::runtime_error("could not create a new evp cipher ctx for CBC encryption");
  }

  if (!EVP_EncryptInit_ex(en_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key),
                          reinterpret_cast<const unsigned char*>(iv))) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("could not initialize evp cipher ctx for CBC encryption");
  }

  // Set padding mode
  if (!EVP_CIPHER_CTX_set_padding(en_ctx, use_padding ? 1 : 0)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("could not set padding mode for CBC encryption");
  }

  if (!EVP_EncryptUpdate(en_ctx, cipher, &len,
                         reinterpret_cast<const unsigned char*>(plaintext),
                         plaintext_len)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("could not update evp cipher ctx for CBC encryption");
  }

  cipher_len += len;

  if (!EVP_EncryptFinal_ex(en_ctx, cipher + len, &len)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("could not finish evp cipher ctx for CBC encryption");
  }

  cipher_len += len;
  EVP_CIPHER_CTX_free(en_ctx);
  return cipher_len;
}

GANDIVA_EXPORT
int32_t aes_decrypt_cbc(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        unsigned char* plaintext, bool use_padding) {
  if (iv_len != 16) {
    throw std::runtime_error("IV length must be 16 bytes for AES-CBC");
  }

  int32_t plaintext_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* de_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_cbc_cipher_algo(key_len);

  if (!de_ctx) {
    throw std::runtime_error("could not create a new evp cipher ctx for CBC decryption");
  }

  if (!EVP_DecryptInit_ex(de_ctx, cipher_algo, nullptr,
                          reinterpret_cast<const unsigned char*>(key),
                          reinterpret_cast<const unsigned char*>(iv))) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("could not initialize evp cipher ctx for CBC decryption");
  }

  // Set padding mode
  if (!EVP_CIPHER_CTX_set_padding(de_ctx, use_padding ? 1 : 0)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("could not set padding mode for CBC decryption");
  }

  if (!EVP_DecryptUpdate(de_ctx, plaintext, &len,
                         reinterpret_cast<const unsigned char*>(ciphertext),
                         ciphertext_len)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("could not update evp cipher ctx for CBC decryption");
  }

  plaintext_len += len;

  if (!EVP_DecryptFinal_ex(de_ctx, plaintext + len, &len)) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("could not finish evp cipher ctx for CBC decryption");
  }

  plaintext_len += len;
  EVP_CIPHER_CTX_free(de_ctx);
  return plaintext_len;
}

}  // namespace gandiva

