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

#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <iomanip>

namespace gandiva {

// Helper function to print hex representation of binary data
inline std::string toHexString(const unsigned char* data, size_t len) {
  std::ostringstream oss;
  for (size_t i = 0; i < len; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
  }
  return oss.str();
}

// Helper function to compute AES-CBC ciphertext using OpenSSL EVP API
// This replaces the shell-based approach which has platform-specific issues
inline std::vector<unsigned char> computeExpectedCiphertext(
    const char* plaintext, int32_t plaintext_len,
    const char* key, int32_t key_len,
    const char* iv, int32_t iv_len,
    bool use_padding) {
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
  }

  // Select cipher based on key length
  const EVP_CIPHER* cipher = nullptr;
  switch (key_len) {
    case 16:
      cipher = EVP_aes_128_cbc();
      break;
    case 24:
      cipher = EVP_aes_192_cbc();
      break;
    case 32:
      cipher = EVP_aes_256_cbc();
      break;
    default:
      EVP_CIPHER_CTX_free(ctx);
      throw std::runtime_error("Unsupported key length");
  }

  std::vector<unsigned char> ciphertext(plaintext_len + 16);  // Max size with padding
  int len = 0;
  int ciphertext_len = 0;

  // Initialize encryption
  if (EVP_EncryptInit_ex(ctx, cipher, nullptr,
                         reinterpret_cast<const unsigned char*>(key),
                         reinterpret_cast<const unsigned char*>(iv)) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptInit_ex failed");
  }

  // Set padding
  EVP_CIPHER_CTX_set_padding(ctx, use_padding ? 1 : 0);

  // Encrypt data
  if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                        reinterpret_cast<const unsigned char*>(plaintext),
                        plaintext_len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptUpdate failed");
  }
  ciphertext_len = len;

  // Finalize encryption
  if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptFinal_ex failed");
  }
  ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);
  ciphertext.resize(ciphertext_len);
  return ciphertext;
}

// Helper function to compare two ciphertexts and print debug info if they differ
inline void compareCiphertexts(const unsigned char* actual, size_t actual_len,
                               const unsigned char* expected, size_t expected_len,
                               const std::string& test_name) {
  if (actual_len != expected_len || std::memcmp(actual, expected, actual_len) != 0) {
    std::cerr << "\n=== CIPHERTEXT MISMATCH in " << test_name << " ===" << std::endl;
    std::cerr << "Expected length: " << expected_len << std::endl;
    std::cerr << "Actual length:   " << actual_len << std::endl;
    std::cerr << "Expected (hex):  " << toHexString(expected, expected_len) << std::endl;
    std::cerr << "Actual (hex):    " << toHexString(actual, actual_len) << std::endl;
    std::cerr << "======================================\n" << std::endl;
  }
}

}  // namespace gandiva

#include <sstream>

