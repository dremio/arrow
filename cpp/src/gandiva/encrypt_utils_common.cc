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

#include "gandiva/encrypt_utils_common.h"
#include <openssl/aes.h>
#include <stdexcept>
#include <cstring>

namespace gandiva {

const EVP_CIPHER* get_cipher_algo(int32_t key_length, const char* cipher_type) {
  if (cipher_type == nullptr) {
    cipher_type = "ecb";
  }

  if (std::strcmp(cipher_type, "ecb") == 0) {
    switch (key_length) {
      case 16:
        return EVP_aes_128_ecb();
      case 24:
        return EVP_aes_192_ecb();
      case 32:
        return EVP_aes_256_ecb();
      default:
        throw std::runtime_error("unsupported key length for AES-ECB");
    }
  } else if (std::strcmp(cipher_type, "cbc") == 0) {
    switch (key_length) {
      case 16:
        return EVP_aes_128_cbc();
      case 24:
        return EVP_aes_192_cbc();
      case 32:
        return EVP_aes_256_cbc();
      default:
        throw std::runtime_error("unsupported key length for AES-CBC");
    }
  } else if (std::strcmp(cipher_type, "gcm") == 0) {
    switch (key_length) {
      case 16:
        return EVP_aes_128_gcm();
      case 24:
        return EVP_aes_192_gcm();
      case 32:
        return EVP_aes_256_gcm();
      default:
        throw std::runtime_error("unsupported key length for AES-GCM");
    }
  } else {
    throw std::runtime_error("unsupported cipher type");
  }
}

}  // namespace gandiva

