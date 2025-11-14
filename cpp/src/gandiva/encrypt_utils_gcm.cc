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

#include "gandiva/encrypt_utils_gcm.h"
#include "gandiva/encrypt_utils_common.h"
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stdexcept>
#include <cstring>
#include <sstream>

namespace gandiva {

namespace {

const EVP_CIPHER* get_gcm_cipher_algo(int32_t key_length) {
  switch (key_length) {
    case 16:
      return EVP_aes_128_gcm();
    case 24:
      return EVP_aes_192_gcm();
    case 32:
      return EVP_aes_256_gcm();
    default: {
      std::ostringstream oss;
      oss << "Unsupported key length for AES-GCM: " << key_length
          << " bytes. Supported lengths: 16, 24, 32 bytes";
      throw std::runtime_error(oss.str());
    }
  }
}

void validate_gcm_iv(int32_t iv_len) {
  if (iv_len != 12) {
    std::ostringstream oss;
    oss << "Invalid IV length for AES-GCM: " << iv_len
        << " bytes. IV must be exactly 12 bytes";
    throw std::runtime_error(oss.str());
  }
}

void validate_gcm_tag_length(int32_t tag_len) {
  if (tag_len < 4 || tag_len > 16) {
    std::ostringstream oss;
    oss << "Invalid tag length for AES-GCM: " << tag_len
        << " bytes. Tag length must be between 4 and 16 bytes";
    throw std::runtime_error(oss.str());
  }
}

}  // namespace

GANDIVA_EXPORT
int32_t aes_encrypt_gcm(const char* plaintext, int32_t plaintext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len,
                        unsigned char* cipher, unsigned char* tag, int32_t tag_len) {
  validate_gcm_iv(iv_len);
  validate_gcm_tag_length(tag_len);

  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_gcm_cipher_algo(key_len);

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

  if (!EVP_CIPHER_CTX_ctrl(en_ctx, EVP_CTRL_GCM_GET_TAG, tag_len, tag)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not extract GCM tag: " + get_openssl_error_string());
  }

  EVP_CIPHER_CTX_free(en_ctx);
  return cipher_len;
}

GANDIVA_EXPORT
int32_t aes_encrypt_gcm_with_aad(const char* plaintext, int32_t plaintext_len,
                                  const char* key, int32_t key_len, const char* iv,
                                  int32_t iv_len, const char* aad, int32_t aad_len,
                                  unsigned char* cipher, unsigned char* tag,
                                  int32_t tag_len) {
  validate_gcm_iv(iv_len);
  validate_gcm_tag_length(tag_len);

  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_gcm_cipher_algo(key_len);

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

  // Set AAD if provided
  if (aad != nullptr && aad_len > 0) {
    if (!EVP_EncryptUpdate(en_ctx, nullptr, &len,
                           reinterpret_cast<const unsigned char*>(aad), aad_len)) {
      EVP_CIPHER_CTX_free(en_ctx);
      throw std::runtime_error("Could not set AAD for encryption: " +
                               get_openssl_error_string());
    }
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

  if (!EVP_CIPHER_CTX_ctrl(en_ctx, EVP_CTRL_GCM_GET_TAG, tag_len, tag)) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw std::runtime_error("Could not extract GCM tag: " + get_openssl_error_string());
  }

  EVP_CIPHER_CTX_free(en_ctx);
  return cipher_len;
}

GANDIVA_EXPORT
int32_t aes_decrypt_gcm(const char* ciphertext, int32_t ciphertext_len, const char* key,
                        int32_t key_len, const char* iv, int32_t iv_len, const char* tag,
                        int32_t tag_len, unsigned char* plaintext) {
  validate_gcm_iv(iv_len);
  validate_gcm_tag_length(tag_len);

  int32_t plaintext_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* de_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_gcm_cipher_algo(key_len);

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

  // Set the tag for verification
  if (!EVP_CIPHER_CTX_ctrl(de_ctx, EVP_CTRL_GCM_SET_TAG, tag_len,
                           const_cast<char*>(tag))) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not set GCM tag for verification: " +
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
    throw std::runtime_error("GCM tag verification failed: " + get_openssl_error_string());
  }

  plaintext_len += len;

  EVP_CIPHER_CTX_free(de_ctx);
  return plaintext_len;
}

GANDIVA_EXPORT
int32_t aes_decrypt_gcm_with_aad(const char* ciphertext, int32_t ciphertext_len,
                                  const char* key, int32_t key_len, const char* iv,
                                  int32_t iv_len, const char* aad, int32_t aad_len,
                                  const char* tag, int32_t tag_len, unsigned char* plaintext) {
  validate_gcm_iv(iv_len);
  validate_gcm_tag_length(tag_len);

  int32_t plaintext_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* de_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_gcm_cipher_algo(key_len);

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

  // Set AAD if provided
  if (aad != nullptr && aad_len > 0) {
    if (!EVP_DecryptUpdate(de_ctx, nullptr, &len,
                           reinterpret_cast<const unsigned char*>(aad), aad_len)) {
      EVP_CIPHER_CTX_free(de_ctx);
      throw std::runtime_error("Could not set AAD for decryption: " +
                               get_openssl_error_string());
    }
  }

  // Set the tag for verification
  if (!EVP_CIPHER_CTX_ctrl(de_ctx, EVP_CTRL_GCM_SET_TAG, tag_len,
                           const_cast<char*>(tag))) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw std::runtime_error("Could not set GCM tag for verification: " +
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
    throw std::runtime_error("GCM tag verification failed: " + get_openssl_error_string());
  }

  plaintext_len += len;

  EVP_CIPHER_CTX_free(de_ctx);
  return plaintext_len;
}

}  // namespace gandiva

