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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "gandiva/execution_context.h"
#include "gandiva/precompiled/types.h"

namespace gandiva {

TEST(TestArrayOps, TestInt32ContainsInt32) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<gdv_int64>(&ctx);
  int32_t data[] = {1, 2, 3, 4};
  int32_t entry_offsets_len = 3;
  int32_t contains_data = 2;
  int32_t entry_validity = 15;
  bool valid = false;

  EXPECT_EQ(array_int32_contains_int32(ctx_ptr, data, entry_offsets_len, &entry_validity,
                                       true, contains_data, true, 0, 3, &valid),
            true);
}

}  // namespace gandiva
