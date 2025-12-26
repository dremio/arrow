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
#include <openssl/rand.h>
#include <stdexcept>
#include <sstream>
#include <cstring>

namespace gandiva {

void generate_random_iv(unsigned char* iv_buffer, int32_t iv_length) {
  if (iv_buffer == nullptr) {
    throw std::runtime_error("IV buffer cannot be null");
  }

  if (iv_length <= 0) {
    std::ostringstream oss;
    oss << "Invalid IV length: " << iv_length << ". IV length must be positive";
    throw std::runtime_error(oss.str());
  }

  // Generate cryptographically secure random bytes using OpenSSL
  int result = RAND_bytes(iv_buffer, iv_length);
  if (result != 1) {
    throw std::runtime_error(
        "Failed to generate random IV: OpenSSL RAND_bytes failed");
  }
}

void extract_iv_from_ciphertext(const char* ciphertext_with_iv, int32_t ciphertext_len,
                                int32_t iv_length, unsigned char* extracted_iv,
                                const char** actual_ciphertext,
                                int32_t* actual_ciphertext_len) {
  if (ciphertext_with_iv == nullptr) {
    throw std::runtime_error("Ciphertext cannot be null");
  }

  if (extracted_iv == nullptr) {
    throw std::runtime_error("Extracted IV buffer cannot be null");
  }

  if (actual_ciphertext == nullptr) {
    throw std::runtime_error("Actual ciphertext output pointer cannot be null");
  }

  if (actual_ciphertext_len == nullptr) {
    throw std::runtime_error("Actual ciphertext length output pointer cannot be null");
  }

  if (iv_length <= 0) {
    std::ostringstream oss;
    oss << "Invalid IV length: " << iv_length << ". IV length must be positive";
    throw std::runtime_error(oss.str());
  }

  if (ciphertext_len < iv_length) {
    std::ostringstream oss;
    oss << "Ciphertext too short to contain IV: ciphertext is " << ciphertext_len
        << " bytes but IV requires " << iv_length << " bytes";
    throw std::runtime_error(oss.str());
  }

  // Extract IV from the beginning of ciphertext
  std::memcpy(extracted_iv, ciphertext_with_iv, iv_length);

  // Set pointer to actual ciphertext (after IV)
  *actual_ciphertext = ciphertext_with_iv + iv_length;
  *actual_ciphertext_len = ciphertext_len - iv_length;
}

}  // namespace gandiva

