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
#include <cstring>

// Test PKCS#7 padding with 16-byte key
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptPkcs7_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, "PKCS7", 5, cipher);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, iv, iv_len,
                                                   "PKCS7", 5, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test PKCS#7 padding with 24-byte key
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptPkcs7_24) {
  auto* key = "12345678abcdefgh12345678";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "some\ntest\nstring";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, "PKCS7", 5, cipher);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, iv, iv_len,
                                                   "PKCS7", 5, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test PKCS#7 padding with 32-byte key
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptPkcs7_32) {
  auto* key = "12345678abcdefgh12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "New\ntest\nstring";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, "PKCS7", 5, cipher);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, iv, iv_len,
                                                   "PKCS7", 5, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test no-padding mode with block-aligned data (16 bytes)
TEST(TestAesCbcEncryptUtils, TestAesEncryptDecryptNoPadding_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "1234567890123456";  // Exactly 16 bytes

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  int32_t cipher_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, "NONE", 4, cipher);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, iv, iv_len,
                                                   "NONE", 4, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test case-insensitive padding mode
TEST(TestAesCbcEncryptUtils, TestCaseInsensitivePadding) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher1[64];
  unsigned char cipher2[64];

  // Test with "pkcs7" (lowercase)
  int32_t cipher1_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                 iv, iv_len, "pkcs7", 5, cipher1);

  // Test with "PKCS7" (uppercase)
  int32_t cipher2_len = gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                                                 iv, iv_len, "PKCS7", 5, cipher2);

  // Both should produce same ciphertext
  EXPECT_EQ(cipher1_len, cipher2_len);
  EXPECT_EQ(std::string(reinterpret_cast<const char*>(cipher1), cipher1_len),
            std::string(reinterpret_cast<const char*>(cipher2), cipher2_len));
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

  ASSERT_THROW({
    gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                             iv, iv_len, "PKCS7", 5, cipher);
  }, std::runtime_error);
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

  ASSERT_THROW({
    gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                             iv, iv_len, "PKCS7", 5, cipher);
  }, std::runtime_error);
}

// Test invalid padding mode
TEST(TestAesCbcEncryptUtils, TestInvalidPaddingMode) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890123456";
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];

  ASSERT_THROW({
    gandiva::aes_encrypt_cbc(to_encrypt, to_encrypt_len, key, key_len,
                             iv, iv_len, "INVALID", 7, cipher);
  }, std::runtime_error);
}

