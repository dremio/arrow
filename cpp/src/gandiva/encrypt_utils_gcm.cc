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
#include "gandiva/encrypt_utils_iv.h"
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

void validate_iv_length_gcm(int32_t iv_len) {
  if (iv_len != GCM_IV_LENGTH) {
    std::ostringstream oss;
    oss << "Invalid IV length for AES-GCM: " << iv_len
        << " bytes. IV must be exactly " << GCM_IV_LENGTH << " bytes";
    throw std::runtime_error(oss.str());
  }
}

void validate_aad_length_gcm(int32_t aad_len) {
  if (aad_len <= 0) {
    throw std::runtime_error("AAD length must be positive when AAD is provided");
  }
}

void validate_ciphertext_with_embedded_iv_gcm(int32_t ciphertext_len) {
  constexpr int32_t MIN_CIPHERTEXT_LEN = GCM_IV_LENGTH + GCM_TAG_LENGTH;

  if (ciphertext_len < MIN_CIPHERTEXT_LEN) {
    std::ostringstream oss;
    oss << "Ciphertext too short for AES-GCM with embedded IV: " << ciphertext_len
        << " bytes. Must be at least " << MIN_CIPHERTEXT_LEN
        << " bytes (12-byte IV + 16-byte tag)";
    throw std::runtime_error(oss.str());
  }
}

void validate_ciphertext_with_tag(int32_t ciphertext_len) {
  if (ciphertext_len < GCM_TAG_LENGTH) {
    throw std::runtime_error(
        "Ciphertext too short for AES-GCM: must be at least 16 bytes for tag");
  }
}

}  // namespace

GANDIVA_EXPORT
int32_t aes_encrypt_gcm(const char* plaintext, int32_t plaintext_len,
                        const char* key, int32_t key_len, const char* iv,
                        int32_t iv_len, const char* aad, int32_t aad_len,
                        unsigned char* cipher) {
  unsigned char iv_buffer[GCM_IV_LENGTH];
  const unsigned char* actual_iv = nullptr;
  bool iv_auto_generated = iv == nullptr;

  if (iv_auto_generated) {
    generate_random_iv(iv_buffer, GCM_IV_LENGTH);
    actual_iv = iv_buffer;
  } else {
    validate_iv_length_gcm(iv_len);
    actual_iv = reinterpret_cast<const unsigned char*>(iv);
  }

  int32_t cipher_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* en_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_gcm_cipher_algo(key_len);

  if (!en_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for encryption: " +
                             get_openssl_error_string());
  }

  try {
    // Only prepend IV to output if it was auto-generated
    // Auto-generated IV: [12-byte IV][ciphertext][16-byte tag]
    // User-supplied IV: [ciphertext][16-byte tag]
    if (iv_auto_generated) {
      std::memcpy(cipher, actual_iv, GCM_IV_LENGTH);
      cipher_len = GCM_IV_LENGTH;
    }

    if (!EVP_EncryptInit_ex(en_ctx, cipher_algo, nullptr,
                            reinterpret_cast<const unsigned char*>(key),
                            actual_iv)) {
      throw std::runtime_error(
          "Could not initialize EVP cipher context for encryption: " +
          get_openssl_error_string());
    }

    // Set IV length for GCM mode
    if (!EVP_CIPHER_CTX_ctrl(en_ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LENGTH, nullptr)) {
      throw std::runtime_error("Could not set GCM IV length: " +
                               get_openssl_error_string());
    }

    // Process AAD if provided
    if (aad != nullptr) {
      validate_aad_length_gcm(aad_len);

      if (!EVP_EncryptUpdate(en_ctx, nullptr, &len,
                             reinterpret_cast<const unsigned char*>(aad), aad_len)) {
        throw std::runtime_error("Could not process AAD for encryption: " +
                                 get_openssl_error_string());
      }
    }

    // Encrypt plaintext (write after IV)
    if (!EVP_EncryptUpdate(en_ctx, cipher + cipher_len, &len,
                           reinterpret_cast<const unsigned char*>(plaintext),
                           plaintext_len)) {
      throw std::runtime_error("Could not update EVP cipher context for encryption: " +
                               get_openssl_error_string());
    }

    cipher_len += len;

    // Finalize encryption
    if (!EVP_EncryptFinal_ex(en_ctx, cipher + cipher_len, &len)) {
      throw std::runtime_error("Could not finalize EVP cipher context for encryption: " +
                               get_openssl_error_string());
    }

    cipher_len += len;

    // Get the authentication tag and append it to ciphertext
    if (!EVP_CIPHER_CTX_ctrl(en_ctx, EVP_CTRL_GCM_GET_TAG, GCM_TAG_LENGTH,
                             cipher + cipher_len)) {
      throw std::runtime_error("Could not get GCM authentication tag: " +
                               get_openssl_error_string());
    }
    cipher_len += GCM_TAG_LENGTH;
  } catch (...) {
    EVP_CIPHER_CTX_free(en_ctx);
    throw;
  }

  EVP_CIPHER_CTX_free(en_ctx);
  return cipher_len;
}

GANDIVA_EXPORT
int32_t aes_decrypt_gcm(const char* ciphertext, int32_t ciphertext_len,
                        const char* key, int32_t key_len, const char* iv,
                        int32_t iv_len, const char* aad, int32_t aad_len,
                        unsigned char* plaintext) {
  unsigned char iv_buffer[GCM_IV_LENGTH];
  const unsigned char* actual_iv = nullptr;
  const char* actual_ciphertext = ciphertext;
  int32_t actual_ciphertext_with_tag_len = ciphertext_len;

  if (iv == nullptr) {
    validate_ciphertext_with_embedded_iv_gcm(ciphertext_len);
    extract_iv_from_ciphertext(ciphertext, ciphertext_len, GCM_IV_LENGTH,
                               iv_buffer, &actual_ciphertext,
                               &actual_ciphertext_with_tag_len);
    actual_iv = iv_buffer;
  } else {
    validate_iv_length_gcm(iv_len);
    validate_ciphertext_with_tag(ciphertext_len);
    actual_iv = reinterpret_cast<const unsigned char*>(iv);
  }

  int32_t plaintext_len = 0;
  int32_t len = 0;
  EVP_CIPHER_CTX* de_ctx = EVP_CIPHER_CTX_new();
  const EVP_CIPHER* cipher_algo = get_gcm_cipher_algo(key_len);

  if (!de_ctx) {
    throw std::runtime_error("Could not create EVP cipher context for decryption: " +
                             get_openssl_error_string());
  }

  try {
    if (!EVP_DecryptInit_ex(de_ctx, cipher_algo, nullptr,
                            reinterpret_cast<const unsigned char*>(key),
                            actual_iv)) {
      throw std::runtime_error(
          "Could not initialize EVP cipher context for decryption: " +
          get_openssl_error_string());
    }

    // Set IV length for GCM mode
    if (!EVP_CIPHER_CTX_ctrl(de_ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LENGTH, nullptr)) {
      throw std::runtime_error("Could not set GCM IV length: " +
                               get_openssl_error_string());
    }

    // Process AAD if provided
    if (aad != nullptr) {
      validate_aad_length_gcm(aad_len);

      if (!EVP_DecryptUpdate(de_ctx, nullptr, &len,
                             reinterpret_cast<const unsigned char*>(aad), aad_len)) {
        throw std::runtime_error("Could not process AAD for decryption: " +
                                 get_openssl_error_string());
      }
    }

    // GCM always has a tag appended, regardless of whether AAD was used
    int32_t ciphertext_without_tag_len = actual_ciphertext_with_tag_len - GCM_TAG_LENGTH;
    const unsigned char* tag = reinterpret_cast<const unsigned char*>(
        actual_ciphertext + ciphertext_without_tag_len);

    // Set the authentication tag
    if (!EVP_CIPHER_CTX_ctrl(de_ctx, EVP_CTRL_GCM_SET_TAG, GCM_TAG_LENGTH,
                             const_cast<unsigned char*>(tag))) {
      throw std::runtime_error("Could not set GCM authentication tag: " +
                               get_openssl_error_string());
    }

    // Decrypt ciphertext
    if (!EVP_DecryptUpdate(de_ctx, plaintext, &len,
                           reinterpret_cast<const unsigned char*>(actual_ciphertext),
                           ciphertext_without_tag_len)) {
      throw std::runtime_error("Could not update EVP cipher context for decryption: " +
                               get_openssl_error_string());
    }

    plaintext_len += len;

    // Finalize decryption (this verifies the tag)
    if (!EVP_DecryptFinal_ex(de_ctx, plaintext + len, &len)) {
      throw std::runtime_error("GCM tag verification failed or decryption error: " +
                               get_openssl_error_string());
    }
    plaintext_len += len;
  } catch (...) {
    EVP_CIPHER_CTX_free(de_ctx);
    throw;
  }

  EVP_CIPHER_CTX_free(de_ctx);
  return plaintext_len;
}

}  // namespace gandiva

