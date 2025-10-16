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

namespace gandiva {

// Helper function to execute OpenSSL command and return output
inline std::vector<unsigned char> runOpenSslCommand(const std::string& cmd) {
  // Suppress stderr to avoid cluttering test output with OpenSSL warnings
  std::string fullCmd = cmd + " 2>/dev/null";
  FILE* pipe = popen(fullCmd.c_str(), "r");
  if (!pipe) {
    throw std::runtime_error("Failed to execute OpenSSL command");
  }

  std::vector<unsigned char> result;
  unsigned char buffer[4096];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), pipe)) > 0) {
    result.insert(result.end(), buffer, buffer + bytes_read);
  }

  pclose(pipe);
  return result;
}

}  // namespace gandiva

