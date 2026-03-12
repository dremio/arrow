#!/usr/bin/env bash

# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

set -ex

arrow_dir=${1}
build_dir=${2}
normalized_arch=$(arch)
case ${normalized_arch} in
  arm64)
    normalized_arch=aarch_64
    ;;
  i386)
    normalized_arch=x86_64
    ;;
esac
# The directory where the final binaries will be stored when scripts finish
dist_dir=${3}

echo "=== Clear output directories and leftovers ==="
# Clear output directories and leftovers
rm -rf ${build_dir}

echo "=== Building Arrow C++ libraries ==="
install_dir=${build_dir}/cpp-install
: ${ARROW_ACERO:=ON}
export ARROW_ACERO
: ${ARROW_BUILD_TESTS:=ON}
: ${ARROW_DATASET:=ON}
export ARROW_DATASET
: ${ARROW_GANDIVA:=ON}
export ARROW_GANDIVA
: ${ARROW_ORC:=ON}
export ARROW_ORC
: ${ARROW_PARQUET:=ON}
: ${ARROW_S3:=ON}
: ${ARROW_USE_CCACHE:=OFF}
: ${CMAKE_BUILD_TYPE:=Release}
: ${CMAKE_UNITY_BUILD:=ON}

if [ "${ARROW_USE_CCACHE}" == "ON" ]; then
  echo "=== ccache statistics before build ==="
  ccache -sv 2>/dev/null || ccache -s
fi

export ARROW_TEST_DATA="${arrow_dir}/testing/data"
export PARQUET_TEST_DATA="${arrow_dir}/cpp/submodules/parquet-testing/data"
export AWS_EC2_METADATA_DISABLED=TRUE

# Determine vcpkg triplet based on architecture
vcpkg_arch=$(arch)
case ${vcpkg_arch} in
  arm64)
    vcpkg_triplet="arm64-osx"
    ;;
  i386|x86_64)
    vcpkg_triplet="x64-osx"
    ;;
  *)
    vcpkg_triplet="arm64-osx"
    ;;
esac

# Set LLVM_DIR to point to vcpkg-installed LLVM if VCPKG_ROOT_LOCAL is set
llvm_dir_arg=""
gandiva_cxx_flags=""
osx_sysroot_arg=""
re2_source_arg="-Dre2_SOURCE=BUNDLED"
if [ -n "${VCPKG_ROOT_LOCAL:-}" ]; then
  vcpkg_installed="${VCPKG_ROOT_LOCAL}/installed/${vcpkg_triplet}"
  llvm_cmake_dir="${vcpkg_installed}/share/llvm"
  if [ -d "${llvm_cmake_dir}" ]; then
    echo "=== Found vcpkg LLVM at ${llvm_cmake_dir} ==="
    llvm_dir_arg="-DLLVM_DIR=${llvm_cmake_dir}"

    # vcpkg's clang needs to know where to find system headers
    # Arrow's GandivaAddBitcode.cmake uses CMAKE_OSX_SYSROOT to set SDKROOT env var
    sdk_path="$(xcrun --show-sdk-path)"
    if [ -d "${sdk_path}" ]; then
      osx_sysroot_arg="-DCMAKE_OSX_SYSROOT=${sdk_path}"
    fi

    # Also pass the C++ standard library include path via ARROW_GANDIVA_PC_CXX_FLAGS
    xcode_path="$(xcode-select -p)"
    cxx_include_path="${xcode_path}/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1"
    if [ -d "${cxx_include_path}" ]; then
      gandiva_cxx_flags="-DARROW_GANDIVA_PC_CXX_FLAGS=-stdlib=libc++;-isystem;${cxx_include_path}"
    fi

    # Use vcpkg's RE2 since it's installed as a dependency of LLVM
    # This ensures ABI compatibility - vcpkg's RE2 uses std::string_view API
    # which matches what vcpkg's LLVM and Abseil expect
    re2_cmake_dir="${vcpkg_installed}/share/re2"
    if [ -d "${re2_cmake_dir}" ]; then
      re2_source_arg="-Dre2_ROOT=${vcpkg_installed}"
    fi
  fi
fi

mkdir -p "${build_dir}/cpp"
pushd "${build_dir}/cpp"

cmake \
  -DARROW_ACERO=${ARROW_ACERO} \
  -DARROW_BUILD_SHARED=OFF \
  -DARROW_BUILD_TESTS=${ARROW_BUILD_TESTS} \
  -DARROW_CSV=${ARROW_DATASET} \
  -DARROW_DATASET=${ARROW_DATASET} \
  -DARROW_SUBSTRAIT=${ARROW_DATASET} \
  -DARROW_DEPENDENCY_USE_SHARED=OFF \
  -DARROW_GANDIVA=${ARROW_GANDIVA} \
  -DARROW_GANDIVA_STATIC_LIBSTDCPP=ON \
  -DARROW_JSON=${ARROW_DATASET} \
  -DARROW_ORC=${ARROW_ORC} \
  -DARROW_PARQUET=${ARROW_PARQUET} \
  -DARROW_S3=${ARROW_S3} \
  -DARROW_USE_CCACHE=${ARROW_USE_CCACHE} \
  -DAWSSDK_SOURCE=BUNDLED \
  -DBoost_SOURCE=BUNDLED \
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
  -DCMAKE_INSTALL_PREFIX=${install_dir} \
  -DCMAKE_UNITY_BUILD=${CMAKE_UNITY_BUILD} \
  -DGTest_SOURCE=BUNDLED \
  ${llvm_dir_arg} \
  ${osx_sysroot_arg} \
  ${gandiva_cxx_flags} \
  -DPARQUET_BUILD_EXAMPLES=OFF \
  -DPARQUET_BUILD_EXECUTABLES=OFF \
  -DPARQUET_REQUIRE_ENCRYPTION=OFF \
  ${re2_source_arg} \
  -GNinja \
  ${arrow_dir}/cpp
cmake --build . --target install

if [ "${ARROW_BUILD_TESTS}" == "ON" ]; then
  # MinIO is required
  exclude_tests="arrow-s3fs-test"
  # unstable
  exclude_tests="${exclude_tests}|arrow-acero-asof-join-node-test"
  exclude_tests="${exclude_tests}|arrow-acero-hash-join-node-test"
  ctest \
    --exclude-regex "${exclude_tests}" \
    --label-regex unittest \
    --output-on-failure \
    --parallel $(sysctl -n hw.ncpu) \
    --timeout 300
fi

popd

export JAVA_JNI_CMAKE_ARGS="-DProtobuf_ROOT=${build_dir}/cpp/protobuf_ep-install"
${arrow_dir}/ci/scripts/java_jni_build.sh \
  ${arrow_dir} \
  ${install_dir} \
  ${build_dir} \
  ${dist_dir}

if [ "${ARROW_USE_CCACHE}" == "ON" ]; then
  echo "=== ccache statistics after build ==="
  ccache -sv 2>/dev/null || ccache -s
fi


echo "=== Checking shared dependencies for libraries ==="
pushd ${dist_dir}
archery linking check-dependencies \
  --allow CoreFoundation \
  --allow Security \
  --allow libSystem \
  --allow libarrow_cdata_jni \
  --allow libarrow_dataset_jni \
  --allow libarrow_orc_jni \
  --allow libc++ \
  --allow libcurl \
  --allow libgandiva_jni \
  --allow libncurses \
  --allow libobjc \
  --allow libz \
  arrow_cdata_jni/${normalized_arch}/libarrow_cdata_jni.dylib \
  arrow_dataset_jni/${normalized_arch}/libarrow_dataset_jni.dylib \
  arrow_orc_jni/${normalized_arch}/libarrow_orc_jni.dylib \
  gandiva_jni/${normalized_arch}/libgandiva_jni.dylib
popd
