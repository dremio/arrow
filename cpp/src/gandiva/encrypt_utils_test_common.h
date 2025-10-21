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

// Helper function to execute OpenSSL command and return output
inline std::vector<unsigned char> runOpenSslCommand(const std::string& cmd) {
  FILE* pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    throw std::runtime_error("Failed to execute OpenSSL command");
  }

  std::vector<unsigned char> result;
  unsigned char buffer[4096];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), pipe)) > 0) {
    result.insert(result.end(), buffer, buffer + bytes_read);
  }

  int status = pclose(pipe);
  if (status != 0) {
    std::cerr << "OpenSSL command failed with status " << status << std::endl;
    std::cerr << "Command was: " << cmd << std::endl;
  }
  return result;
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

