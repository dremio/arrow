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

#ifndef GANDIVA_ENCRYPT_UTILS_IV_H
#define GANDIVA_ENCRYPT_UTILS_IV_H

#include <cstdint>
#include "gandiva/visibility.h"

namespace gandiva {

/**
 * Generate a cryptographically secure random initialization vector (IV)
 * using OpenSSL's RAND_bytes.
 *
 * @param iv_buffer Output buffer to store the generated IV
 * @param iv_length Length of IV to generate in bytes (typically 12 for GCM, 16 for CBC)
 * @throws std::runtime_error if random number generation fails
 */
GANDIVA_EXPORT
void generate_random_iv(unsigned char* iv_buffer, int32_t iv_length);

/**
 * Extract IV from the beginning of ciphertext and return pointer to actual ciphertext.
 * This is a helper function for decrypt operations when IV is embedded in the ciphertext.
 *
 * @param ciphertext_with_iv Pointer to ciphertext with IV prepended
 * @param ciphertext_len Total length including IV
 * @param iv_length Expected IV length (12 for GCM, 16 for CBC)
 * @param extracted_iv Output buffer to store extracted IV (must be at least iv_length bytes)
 * @param actual_ciphertext Output pointer to the actual ciphertext (after IV)
 * @param actual_ciphertext_len Output length of actual ciphertext (without IV)
 * @throws std::runtime_error if ciphertext is too short to contain IV
 */
GANDIVA_EXPORT
void extract_iv_from_ciphertext(const char* ciphertext_with_iv, int32_t ciphertext_len,
                                int32_t iv_length, unsigned char* extracted_iv,
                                const char** actual_ciphertext,
                                int32_t* actual_ciphertext_len);

}  // namespace gandiva

#endif  // GANDIVA_ENCRYPT_UTILS_IV_H

