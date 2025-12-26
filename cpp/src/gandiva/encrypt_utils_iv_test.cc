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

#include "gandiva/encrypt_utils_iv.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>

// Test that multiple IV generations produce different values
TEST(TestIVUtils, TestGenerateRandomIvUniqueness) {
  unsigned char iv1[gandiva::GCM_IV_LENGTH];
  unsigned char iv2[gandiva::GCM_IV_LENGTH];

  gandiva::generate_random_iv(iv1, gandiva::GCM_IV_LENGTH);
  gandiva::generate_random_iv(iv2, gandiva::GCM_IV_LENGTH);

  EXPECT_NE(0, std::memcmp(iv1, iv2, gandiva::GCM_IV_LENGTH));
}

// Test that generated IV has the correct length
TEST(TestIVUtils, TestGenerateRandomIvLength) {
  unsigned char iv[gandiva::GCM_IV_LENGTH];

  // Generate IV with GCM_IV_LENGTH (12 bytes)
  ASSERT_NO_THROW(gandiva::generate_random_iv(iv, gandiva::GCM_IV_LENGTH));

  // Verify that the function accepts the correct length without throwing
  // The actual length verification is implicit - if the buffer is correctly
  // filled, no out-of-bounds access occurs

  // Also test with CBC_IV_LENGTH (16 bytes)
  unsigned char iv_cbc[gandiva::CBC_IV_LENGTH];
  ASSERT_NO_THROW(gandiva::generate_random_iv(iv_cbc, gandiva::CBC_IV_LENGTH));
}

// Test error handling for null buffer
TEST(TestIVUtils, TestGenerateRandomIvNullBuffer) {
  EXPECT_THROW(gandiva::generate_random_iv(nullptr, gandiva::GCM_IV_LENGTH),
               std::runtime_error);
}

// Test error handling for invalid length
TEST(TestIVUtils, TestGenerateRandomIvInvalidLength) {
  unsigned char iv[16];
  EXPECT_THROW(gandiva::generate_random_iv(iv, 0), std::runtime_error);
  EXPECT_THROW(gandiva::generate_random_iv(iv, -1), std::runtime_error);
}

// Test extracting GCM IV from ciphertext
TEST(TestIVUtils, TestExtractIvFromCiphertextGcm) {
  // Create test data: [12-byte IV][ciphertext]
  const char test_data[] = "123456789012CIPHERTEXT_DATA";
  const int32_t total_len = 28;  // 12 + 16
  
  unsigned char extracted_iv[gandiva::GCM_IV_LENGTH];
  const char* actual_ciphertext = nullptr;
  int32_t actual_ciphertext_len = 0;
  
  ASSERT_NO_THROW(gandiva::extract_iv_from_ciphertext(
      test_data, total_len, gandiva::GCM_IV_LENGTH, extracted_iv,
      &actual_ciphertext, &actual_ciphertext_len));
  
  // Verify IV was extracted correctly
  EXPECT_EQ(0, std::memcmp(extracted_iv, "123456789012", gandiva::GCM_IV_LENGTH));
  
  // Verify ciphertext pointer and length
  EXPECT_EQ(actual_ciphertext, test_data + gandiva::GCM_IV_LENGTH);
  EXPECT_EQ(actual_ciphertext_len, 16);
  EXPECT_EQ(0, std::memcmp(actual_ciphertext, "CIPHERTEXT_DATA", 15));
}

// Test extracting CBC IV from ciphertext
TEST(TestIVUtils, TestExtractIvFromCiphertextCbc) {
  // Create test data: [16-byte IV][ciphertext]
  const char test_data[] = "1234567890123456CIPHERTEXT_DATA_HERE";
  const int32_t total_len = 37;  // 16 + 21
  
  unsigned char extracted_iv[gandiva::CBC_IV_LENGTH];
  const char* actual_ciphertext = nullptr;
  int32_t actual_ciphertext_len = 0;
  
  ASSERT_NO_THROW(gandiva::extract_iv_from_ciphertext(
      test_data, total_len, gandiva::CBC_IV_LENGTH, extracted_iv,
      &actual_ciphertext, &actual_ciphertext_len));
  
  // Verify IV was extracted correctly
  EXPECT_EQ(0, std::memcmp(extracted_iv, "1234567890123456", gandiva::CBC_IV_LENGTH));
  
  // Verify ciphertext pointer and length
  EXPECT_EQ(actual_ciphertext, test_data + gandiva::CBC_IV_LENGTH);
  EXPECT_EQ(actual_ciphertext_len, 21);
  EXPECT_EQ(0, std::memcmp(actual_ciphertext, "CIPHERTEXT_DATA_HERE", 20));
}

// Test error handling for ciphertext too short
TEST(TestIVUtils, TestExtractIvFromCiphertextTooShort) {
  const char test_data[] = "SHORT";
  unsigned char extracted_iv[gandiva::GCM_IV_LENGTH];
  const char* actual_ciphertext = nullptr;
  int32_t actual_ciphertext_len = 0;
  
  EXPECT_THROW(gandiva::extract_iv_from_ciphertext(
      test_data, 5, gandiva::GCM_IV_LENGTH, extracted_iv,
      &actual_ciphertext, &actual_ciphertext_len),
      std::runtime_error);
}

// Test error handling for null inputs
TEST(TestIVUtils, TestExtractIvFromCiphertextNullInputs) {
  const char test_data[] = "1234567890123456CIPHERTEXT";
  unsigned char extracted_iv[16];
  const char* actual_ciphertext = nullptr;
  int32_t actual_ciphertext_len = 0;
  
  // Null ciphertext
  EXPECT_THROW(gandiva::extract_iv_from_ciphertext(
      nullptr, 27, 16, extracted_iv, &actual_ciphertext, &actual_ciphertext_len),
      std::runtime_error);
  
  // Null extracted_iv buffer
  EXPECT_THROW(gandiva::extract_iv_from_ciphertext(
      test_data, 27, 16, nullptr, &actual_ciphertext, &actual_ciphertext_len),
      std::runtime_error);
  
  // Null actual_ciphertext pointer
  EXPECT_THROW(gandiva::extract_iv_from_ciphertext(
      test_data, 27, 16, extracted_iv, nullptr, &actual_ciphertext_len),
      std::runtime_error);
  
  // Null actual_ciphertext_len pointer
  EXPECT_THROW(gandiva::extract_iv_from_ciphertext(
      test_data, 27, 16, extracted_iv, &actual_ciphertext, nullptr),
      std::runtime_error);
}

