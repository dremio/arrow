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
#include <gmock/gmock.h>
#include <cstring>

// Test IV-only GCM with 16-byte key (user-supplied IV)
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptIvOnly_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";  // 12-byte IV
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, nullptr, 0, cipher);

  // Output format: [12-byte IV][ciphertext][16-byte tag]
  EXPECT_EQ(cipher_len, to_encrypt_len + 12 + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 12));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   nullptr, 0, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test IV + AAD GCM with 16-byte key (user-supplied IV)
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptWithAad_16) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "some test string";
  auto* aad = "additional authenticated data";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  auto aad_len = static_cast<int32_t>(strlen(aad));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, aad, aad_len, cipher);

  // Output format: [12-byte IV][ciphertext][16-byte tag]
  EXPECT_EQ(cipher_len, to_encrypt_len + 12 + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 12));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   aad, aad_len, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test IV-only GCM with 24-byte key (user-supplied IV)
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptIvOnly_24) {
  auto* key = "12345678abcdefgh12345678";
  auto* iv = "123456789012";
  auto* to_encrypt = "test data";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, nullptr, 0, cipher);

  // Output format: [12-byte IV][ciphertext][16-byte tag]
  EXPECT_EQ(cipher_len, to_encrypt_len + 12 + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 12));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   nullptr, 0, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test IV-only GCM with 32-byte key (user-supplied IV)
TEST(TestAesGcmEncryptUtils, TestAesEncryptDecryptIvOnly_32) {
  auto* key = "12345678abcdefgh12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "another test";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, nullptr, 0, cipher);

  // Output format: [12-byte IV][ciphertext][16-byte tag]
  EXPECT_EQ(cipher_len, to_encrypt_len + 12 + 16);

  // Verify IV is prepended
  EXPECT_EQ(0, std::memcmp(cipher, iv, 12));

  unsigned char decrypted[128];
  // Pass NULL IV to extract from ciphertext (since encrypt prepended it)
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   nullptr, 0, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test tag verification failure (user-supplied IV)
TEST(TestAesGcmEncryptUtils, TestTagVerificationFailure) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, nullptr, 0, cipher);

  // Corrupt the tag (last byte)
  cipher[cipher_len - 1] ^= 0xFF;

  unsigned char decrypted[128];
  EXPECT_THROW(gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                        cipher_len, key, key_len, iv, iv_len,
                                        nullptr, 0, decrypted),
               std::runtime_error);
}

// Test invalid IV length (non-12-byte IV should fail)
TEST(TestAesGcmEncryptUtils, TestInvalidIvLength) {
  auto* key = "12345678abcdefgh";
  auto* iv = "1234567890";  // 10-byte IV (invalid, must be exactly 12)
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  EXPECT_THROW(gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                        iv, iv_len, nullptr, 0, cipher),
               std::runtime_error);
}

// Test NULL IV with auto-generation (encrypt and decrypt round-trip)
TEST(TestAesGcmEncryptUtils, TestNullIvAutoGeneration) {
  auto* key = "12345678abcdefgh";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  // Encrypt with NULL IV (auto-generate)
  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                nullptr, 0, nullptr, 0, cipher);

  // Output format: [12-byte IV][ciphertext][16-byte tag]
  EXPECT_EQ(cipher_len, to_encrypt_len + 12 + 16);

  // Decrypt with NULL IV (extract from ciphertext)
  unsigned char decrypted[128];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   nullptr, 0, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test NULL IV with AAD
TEST(TestAesGcmEncryptUtils, TestNullIvWithAad) {
  auto* key = "12345678abcdefgh";
  auto* to_encrypt = "some test string";
  auto* aad = "additional authenticated data";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  auto aad_len = static_cast<int32_t>(strlen(aad));
  unsigned char cipher[128];

  // Encrypt with NULL IV and AAD
  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                nullptr, 0, aad, aad_len, cipher);

  // Output format: [12-byte IV][ciphertext][16-byte tag]
  EXPECT_EQ(cipher_len, to_encrypt_len + 12 + 16);

  // Decrypt with NULL IV and AAD
  unsigned char decrypted[128];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   aad, aad_len, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test NULL IV decrypt with user-supplied IV encrypt (backward compatibility)
TEST(TestAesGcmEncryptUtils, TestNullIvDecryptWithSuppliedIvEncrypt) {
  auto* key = "12345678abcdefgh";
  auto* iv = "123456789012";
  auto* to_encrypt = "some test string";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto iv_len = static_cast<int32_t>(strlen(iv));
  auto to_encrypt_len = static_cast<int32_t>(strlen(to_encrypt));
  unsigned char cipher[128];

  // Encrypt with user-supplied IV (IV will be prepended)
  int32_t cipher_len = gandiva::aes_encrypt_gcm(to_encrypt, to_encrypt_len, key, key_len,
                                                iv, iv_len, nullptr, 0, cipher);

  // Decrypt with NULL IV (extract IV from ciphertext)
  unsigned char decrypted[128];
  int32_t decrypted_len = gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len, nullptr, 0,
                                                   nullptr, 0, decrypted);

  EXPECT_EQ(std::string(to_encrypt, to_encrypt_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test decrypt with too-short ciphertext (NULL IV case)
TEST(TestAesGcmEncryptUtils, TestDecryptTooShortCiphertext) {
  auto* key = "12345678abcdefgh";
  auto key_len = static_cast<int32_t>(strlen(key));

  // Ciphertext too short: only 20 bytes (needs at least 28: 12 IV + 16 tag)
  unsigned char short_cipher[20] = {0};
  unsigned char decrypted[128];

  EXPECT_THROW(gandiva::aes_decrypt_gcm(reinterpret_cast<const char*>(short_cipher),
                                        20, key, key_len, nullptr, 0,
                                        nullptr, 0, decrypted),
               std::runtime_error);
}

