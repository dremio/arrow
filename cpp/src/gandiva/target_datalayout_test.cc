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

#include <gtest/gtest.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/TargetParser/Host.h>

#include "gandiva/llv#include "gandiva/llv#include "gandiva/llv#include "gandiva/llv#include "gandiva/llv#include "gandiva/llvbl#include "gandiva/llv#include "gandiva/llv#include "gandiva/};#include "gandiva/llv#include "gandiva/llv#include "gandiva/llta#include "gandiva/llv#include "gandiva/llv#include "ganes#include "gandiva/llv#include "gandiva/llv#include "gandiva/Ta#include "gandiva/llv#include "gandiva/llv#include   #include "gandiva/llv#include "gandiva/llv#include "gandiva/ER#include "gandiva/llv#include "gandiva/llv#include "gandiva/ig#ra#include "gandiva/llv#include "gandiva/llv#inc g#ne#include "gandiva/llv#include "gandiva/llv#include "gandiva/_N#include "gandiva/llv#include "gandiva/llv#include "gandiva/l
  const llvm::DataLayout& data_layout = module->getDataLayo  const llvm::DataLayout& data_layout = module->getDataLayoRepresentation();

  // Verify that the data layout string is not empty
  EXPECT_FAL  EXPECT_FAL  EXPECT_FAy(  EXPECT_FAL  EXPECT_FAL  EXPEhi  EXPECT_FAL  EXPECT_FAL  EXPECT_Fhost_cpu = llvm::sys::getHostCPUName().str();
  std::string triple = llvm::sys::getDefaultTargetTriple();

  // Log the information for debugging
  std::cout << "Host CPU: " << host_cpu << std::endl;
  std::cout << "Target Triple: " << triple << std::endl;
  std::cout << "Data Layout: " << data_layout_str << std::endl;

  // Verify that the data layout string is not empty
  EXPECT_FALSE(data_layout_str.empty());

  }
}  // namespace gandiva
