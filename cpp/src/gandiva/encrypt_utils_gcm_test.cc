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

#include <gtest/gtest.h>
#include <cstring>

// Test encryption/decryption without AAD with 16-byte key
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptNoAad_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";  // 12 bytes for GCM
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];
  unsigned char tag[16];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, cipher, tag, 16);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(
      reinterpret_cast<const char*>(cipher), cipher_len, key, key_len, iv, iv_len,
      reinterpret_cast<const char*>(tag), 16, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test encryption/decryption without AAD with 24-byte key
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptNoAad_24) {
  auto* key = "12345678abcdefgh12345678";
  auto* iv = "123456789012";
  auto* to_encrypt = "some\ntest\nstring";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];
  unsigned char tag[16];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, cipher, tag, 16);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(
      reinterpret_cast<const char*>(cipher), cipher_len, key, key_len, iv, iv_len,
      reinterpret_cast<const char*>(tag), 16, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test encryption/decryption without AAD with 32-byte key
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptNoAad_32) {
  auto* key = "12345678abcdefgh12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "New\ntest\nstring";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];
  unsigned char tag[16];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, cipher, tag, 16);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(
      reinterpret_cast<const char*>(cipher), cipher_len, key, key_len, iv, iv_len,
      reinterpret_cast<const char*>(tag), 16, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test encryption/decryption with AAD
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptWithAad) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "secret message";
  auto* aad = "additional data";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  auto aad_len = static_cast<int32_t>(strlen(aad));
  unsigned char cipher[64];
  unsigned char tag[16];

  int32_t cipher_len = gandiva::aes_encrypt_gcm_with_aad(
      to_encrypt, to_encrypt_len, key, key_len, iv, iv_len, aad, aad_len, cipher, tag, 16);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm_with_aad(
      reinterpret_cast<const char*>(cipher), cipher_len, key, key_len, iv, iv_len, aad,
      aad_len, reinterpret_cast<const char*>(tag), 16, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test invalid IV length (not 12 bytes)
TEST(TestAesGcmEncryptUtils, TestInvalidIVLength) {
  auto* key = "12345678abcdefgh";
  auto* iv = "short";  // Too short
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];
  unsigned char tag[16];

  ASSERT_THROW(
      { gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len, iv, iv_len,
                                 cipher, tag, 16); },
      std::runtime_error);
}

// Test invalid key length
TEST(TestAesGcmEncryptUtils, TestInvalidKeyLength) {
  auto* key = "short";  // Invalid key length
  auto* iv = "123456789012";
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];
  unsigned char tag[16];

  ASSERT_THROW(
      { gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len, iv, iv_len,
                                 cipher, tag, 16); },
      std::runtime_error);
}

// Test invalid tag length
TEST(TestAesGcmEncryptUtils, TestInvalidTagLength) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[64];
  unsigned char tag[32];

  ASSERT_THROW(
      { gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len, iv, iv_len,
                                 cipher, tag, 32); },  // Tag too long
      std::runtime_error);
}

// Test empty plaintext
TEST(TestAesGcmEncryptUtils, TestEmptyPlaintext) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = 0;
  unsigned char cipher[64];
  unsigned char tag[16];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, cipher, tag, 16);

  unsigned char decrypted[64];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(
      reinterpret_cast<const char*>(cipher), cipher_len, key, key_len, iv, iv_len,
      reinterpret_cast<const char*>(tag), 16, decrypted);

  EXPECT_EQ(0, decrypted_len);
}

