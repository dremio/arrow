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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>

// Test PKCS#7 padding with 16-byte key (user-supplied IV)
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptPkcs7_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, true, cipher);

  // Output format: [16-byte IV][ciphertext]
  // Ciphertext includes padding, so it's rounded up to next 16-byte block
  EXPECT_GE(cipher_len, to_encrypt_len + 16);  // At least IV + plaintext

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 16));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   true, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test PKCS#7 padding with 24-byte key (user-supplied IV)
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptPkcs7_24) {
  auto* key = "12345678abcdefgh12345678";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "some\ntest\nstring";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, true, cipher);

  // Output format: [16-byte IV][ciphertext]
  EXPECT_GE(cipher_len, to_encrypt_len + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 16));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   true, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test PKCS#7 padding with 32-byte key (user-supplied IV)
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptPkcs7_32) {
  auto* key = "12345678abcdefgh12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "New\ntest\nstring";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, true, cipher);

  // Output format: [16-byte IV][ciphertext]
  EXPECT_GE(cipher_len, to_encrypt_len + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 16));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   true, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test no-padding mode with block-aligned data (16 bytes, user-supplied IV)
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptNoPadding_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "1234567890123456";  // Exactly 16 bytes

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, false, cipher);

  // Output format: [16-byte IV][ciphertext]
  // No padding, so ciphertext is exactly 16 bytes
  EXPECT_EQ(cipher_len, 16 + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 16));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   false, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test invalid IV length
TEST(TestAesCbcEncryptUtils, TestInvalidIVLength) {
  auto* key = "12345678abcdefgh";
  auto* iv = "short";  // Too short
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  try {
    gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                             iv, iv_len, true, cipher);
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error& e) {
    EXPECT_THAT(e.what(), testing::HasSubstr("Invalid IV length for AES-CBC"));
  }
}

// Test invalid key length
TEST(TestAesCbcEncryptUtils, TestInvalidKeyLength) {
  auto* key = "short";  // Too short
  auto* iv = "1234567890123456";
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  try {
    gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                             iv, iv_len, true, cipher);
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error& e) {
    EXPECT_THAT(e.what(), testing::HasSubstr("Unsupported key length for AES-CBC"));
  }
}

// Test NULL IV with auto-generation (encrypt and decrypt round-trip)
TEST(TestAesCbcEncryptUtils, TestNullIvAutoGeneration) {
  auto* key = "12345678abcdefgh";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  // Encrypt with NULL IV (auto-generate)
  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                nullptr, 0, true, cipher);

  // Output format: [16-byte IV][ciphertext with padding]
  EXPECT_GE(cipher_len, to_encrypt_len + 16);

  // Decrypt with NULL IV (extract from ciphertext)
  unsigned char decrypted[128];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   true, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test NULL IV with no padding
TEST(TestAesCbcEncryptUtils, TestNullIvNoPadding) {
  auto* key = "12345678abcdefgh";
  auto* to_encrypt = "1234567890123456";  // Exactly 16 bytes

  auto key_len = static_cast<int32_t>(strlen(key));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  // Encrypt with NULL IV and no padding
  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                nullptr, 0, false, cipher);

  // Output format: [16-byte IV][16-byte ciphertext]
  EXPECT_EQ(cipher_len, 16 + 16);

  // Decrypt with NULL IV and no padding
  unsigned char decrypted[128];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   false, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test NULL IV decrypt with user-supplied IV encrypt (backward compatibility)
TEST(TestAesCbcEncryptUtils, TestNullIvDecryptWithSuppliedIvEncrypt) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  // Encrypt with user-supplied IV (IV will be prepended)
  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, true, cipher);

  // Decrypt with NULL IV (extract IV from ciphertext)
  unsigned char decrypted[128];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   true, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test decrypt with too-short ciphertext (NULL IV case)
TEST(TestAesCbcEncryptUtils, TestDecryptTooShortCiphertext) {
  auto* key = "12345678abcdefgh";
  auto key_len = static_cast<int32_t>(strlen(key));

  // Ciphertext too short: only 20 bytes (needs at least 32: 16 IV + 16 min block)
  unsigned char short_cipher[20] = {0};
  unsigned char decrypted[128];

  EXPECT_THROW(gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(short_cipher),
                                        20, key, key_len, nullptr, 0,
                                        true, decrypted),
               std::runtime_error);
}



