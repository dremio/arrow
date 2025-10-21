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
#include <cstdlib>
#include <vector>

#include "arrow/util/string.h"
#include "gandiva/encrypt_utils_test_common.h"

using gandiva::runOpenSslCommand;
using gandiva::compareCiphertexts;

// Test AES-128-CBC encryption
TEST(TestAesCbcEncryptUtils, TestAesCbcEncrypt16) {
  const char* key = "12345678abcdefgh";
  const char* plaintext = "Hello World!";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[64];

  // Get expected ciphertext from OpenSSL CLI
  auto cli_cipher = runOpenSslCommand(
      (std::ostringstream() << "echo -n '" << plaintext << "' |"
                            << " openssl enc -aes-128-cbc"
                            << " -K " << arrow::HexEncode(key, key_len)
                            << " -iv " << arrow::HexEncode(iv, 16))
          .str());

  // Encrypt
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);

  // 12 bytes plaintext + 4 bytes PKCS7 padding = 16 bytes
  EXPECT_EQ(cipher_len, 16);
  compareCiphertexts(cipher, cipher_len, cli_cipher.data(), cli_cipher.size(),
                     "TestAesCbcEncrypt16");
  EXPECT_EQ(0, std::memcmp(cipher, cli_cipher.data(), cipher_len));
}

// Test AES-128-CBC encryption/decryption
TEST(TestAesCbcEncryptUtils, TestAesCbcDecrypt16) {
  const char* key = "12345678abcdefgh";
  const char* plaintext = "Hello World!";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[64];
  unsigned char decrypted[64];

  // Encrypt
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);
  EXPECT_EQ(cipher_len, 16);

  // Decrypt
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len,
                                                   iv, 16, decrypted);

  EXPECT_EQ(decrypted_len, plaintext_len);
  EXPECT_EQ(std::string(plaintext, plaintext_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test AES-192-CBC encryption
TEST(TestAesCbcEncryptUtils, TestAesCbcEncrypt24) {
  const char* key = "12345678abcdefgh12345678";
  const char* plaintext = "Test data for 24-byte key";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[128];

  auto cli_cipher = runOpenSslCommand(
      (std::ostringstream() << "echo -n '" << plaintext << "' |"
                            << " openssl enc -aes-192-cbc"
                            << " -K " << arrow::HexEncode(key, key_len)
                            << " -iv " << arrow::HexEncode(iv, 16))
          .str());

  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);

  // 25 bytes plaintext + 7 bytes PKCS7 padding = 32 bytes
  EXPECT_EQ(cipher_len, 32);
  compareCiphertexts(cipher, cipher_len, cli_cipher.data(), cli_cipher.size(),
                     "TestAesCbcEncrypt24");
  EXPECT_EQ(0, std::memcmp(cipher, cli_cipher.data(), cipher_len));
}

// Test AES-192-CBC encryption/decryption
TEST(TestAesCbcEncryptUtils, TestAesCbcDecrypt24) {
  const char* key = "12345678abcdefgh12345678";
  const char* plaintext = "Test data for 24-byte key";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[128];
  unsigned char decrypted[128];

  // Encrypt
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);
  EXPECT_EQ(cipher_len, 32);

  // Decrypt
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len,
                                                   iv, 16, decrypted);

  EXPECT_EQ(decrypted_len, plaintext_len);
  EXPECT_EQ(std::string(plaintext, plaintext_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test AES-256-CBC encryption
TEST(TestAesCbcEncryptUtils, TestAesCbcEncrypt32) {
  const char* key = "12345678abcdefgh12345678abcdefgh";
  const char* plaintext = "Test data for 32-byte key";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[128];

  auto cli_cipher = runOpenSslCommand(
      (std::ostringstream() << "echo -n '" << plaintext << "' |"
                            << " openssl enc -aes-256-cbc"
                            << " -K " << arrow::HexEncode(key, key_len)
                            << " -iv " << arrow::HexEncode(iv, 16))
          .str());

  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);

  // 25 bytes plaintext + 7 bytes PKCS7 padding = 32 bytes
  EXPECT_EQ(cipher_len, 32);
  compareCiphertexts(cipher, cipher_len, cli_cipher.data(), cli_cipher.size(),
                     "TestAesCbcEncrypt32");
  EXPECT_EQ(0, std::memcmp(cipher, cli_cipher.data(), cipher_len));
}

// Test AES-256-CBC encryption/decryption
TEST(TestAesCbcEncryptUtils, TestAesCbcDecrypt32) {
  const char* key = "12345678abcdefgh12345678abcdefgh";
  const char* plaintext = "Test data for 32-byte key";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[128];
  unsigned char decrypted[128];

  // Encrypt
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);
  EXPECT_EQ(cipher_len, 32);

  // Decrypt
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len,
                                                   iv, 16, decrypted);

  EXPECT_EQ(decrypted_len, plaintext_len);
  EXPECT_EQ(std::string(plaintext, plaintext_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test invalid IV length error handling
TEST(TestAesCbcEncryptUtils, TestAesCbcInvalidIvLength) {
  auto* key = "12345678abcdefgh";
  auto* plaintext = "test";
  unsigned char iv[8];  // Invalid IV length
  unsigned char cipher[64];
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));

  ASSERT_THROW({
    gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                             reinterpret_cast<const char*>(iv), 8, cipher);
  }, std::runtime_error);
}

// Test AES-128-CBC encryption without padding
TEST(TestAesCbcEncryptUtils, TestAesCbcNoPaddingEncrypt16) {
  // Plaintext must be multiple of 16 bytes for no-padding mode
  const char* key = "12345678abcdefgh";
  const char* plaintext = "Hello World!!!!!";  // 16 bytes exactly
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[64];

  auto cli_cipher = runOpenSslCommand(
      (std::ostringstream() << "echo -n '" << plaintext << "' |"
                            << " openssl enc -aes-128-cbc -nopad"
                            << " -K " << arrow::HexEncode(key, key_len)
                            << " -iv " << arrow::HexEncode(iv, 16))
          .str());

  // Encrypt without padding
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher,
                                                false);  // use_padding = false

  // 16 bytes plaintext, no padding = 16 bytes ciphertext
  EXPECT_EQ(cipher_len, 16);
  compareCiphertexts(cipher, cipher_len, cli_cipher.data(), cli_cipher.size(),
                     "TestAesCbcNoPaddingEncrypt16");
  EXPECT_EQ(0, std::memcmp(cipher, cli_cipher.data(), cipher_len));
}

// Test AES-128-CBC encryption/decryption without padding
TEST(TestAesCbcEncryptUtils, TestAesCbcNoPaddingRoundTrip16) {
  const char* key = "12345678abcdefgh";
  const char* plaintext = "Hello World!!!!!";  // 16 bytes exactly
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[64];
  unsigned char decrypted[64];

  // Encrypt without padding
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher,
                                                false);
  EXPECT_EQ(cipher_len, 16);

  // Decrypt without padding
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len,
                                                   iv, 16,
                                                   decrypted, false);

  EXPECT_EQ(decrypted_len, plaintext_len);
  EXPECT_EQ(std::string(plaintext, plaintext_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}

// Test AES-256-CBC encryption/decryption without padding
TEST(TestAesCbcEncryptUtils, TestAesCbcNoPaddingRoundTrip32) {
  const char* key = "12345678abcdefgh12345678abcdefgh";
  const char plaintext[] = "Test data for 32-byte key!!!!!!!";  // 32 bytes exactly (no null terminator counted)
  const char* iv = "1234567890123456";

  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = 32;  // Explicitly 32 bytes
  unsigned char cipher[128];
  unsigned char decrypted[128];

  // Encrypt without padding
  int32_t cipher_len = gandiva::aes_encrypt_cbc(plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher,
                                                false);
  EXPECT_EQ(cipher_len, 32);

  // Decrypt without padding
  int32_t decrypted_len = gandiva::aes_decrypt_cbc(reinterpret_cast<const char*>(cipher),
                                                   cipher_len, key, key_len,
                                                   iv, 16,
                                                   decrypted, false);

  EXPECT_EQ(decrypted_len, plaintext_len);
  EXPECT_EQ(std::string(plaintext, plaintext_len),
            std::string(reinterpret_cast<const char*>(decrypted), decrypted_len));
}


// Negative test: Verify encryption fails when plaintext differs from expected
TEST(TestAesCbcEncryptUtils, TestAesCbcEncrypt16Negative) {
  const char* key = "12345678abcdefgh";
  const char* plaintext = "Hello World!";
  const char* wrong_plaintext = "Wrong Plaintext";  // Different plaintext
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext));
  unsigned char cipher[64];

  // Get expected ciphertext using correct plaintext
  auto cli_cipher = runOpenSslCommand(
      (std::ostringstream() << "echo -n '" << plaintext << "' |"
                            << " openssl enc -aes-128-cbc"
                            << " -K " << arrow::HexEncode(key, key_len)
                            << " -iv " << arrow::HexEncode(iv, 16))
          .str());

  // Encrypt using wrong plaintext
  int32_t cipher_len = gandiva::aes_encrypt_cbc(wrong_plaintext, plaintext_len, key, key_len,
                                                iv, 16, cipher);

  // Ciphertexts should NOT match
  EXPECT_EQ(cipher_len, 16);
  EXPECT_NE(0, std::memcmp(cipher, cli_cipher.data(), cipher_len));
}

// Negative test: Verify encryption with different plaintext produces different ciphertext
TEST(TestAesCbcEncryptUtils, TestAesCbcEncrypt16NegativeDecrypt) {
  const char* key = "12345678abcdefgh";
  const char* plaintext1 = "Hello World!";
  const char* plaintext2 = "Different Text!";
  const char* iv = "1234567890123456";
  auto key_len = static_cast<int32_t>(strlen(key));
  auto plaintext_len = static_cast<int32_t>(strlen(plaintext1));
  unsigned char cipher1[64];
  unsigned char cipher2[64];

  // Encrypt plaintext1
  int32_t cipher_len1 = gandiva::aes_encrypt_cbc(plaintext1, plaintext_len, key, key_len,
                                                 iv, 16, cipher1);
  EXPECT_EQ(cipher_len1, 16);

  // Encrypt plaintext2
  int32_t cipher_len2 = gandiva::aes_encrypt_cbc(plaintext2, plaintext_len, key, key_len,
                                                 iv, 16, cipher2);
  EXPECT_EQ(cipher_len2, 16);

  // Ciphertexts should be different
  EXPECT_NE(0, std::memcmp(cipher1, cipher2, cipher_len1));
}
