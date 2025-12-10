vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO google/brotli
    REF "v${VERSION}"
    SHA512 0b5c374417938eae11101995a373d30b1c91212e901e787b6e4b620195247ee754a3a4abda82793582be0b5fbe9dafca2da0b90e1341b3b91641c27cde6d42de1
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBROTLI_BUNDLED_MODE=ON
)

vcpkg_cmake_install()

vcpkg_copy_pdbs()

# Remove unnecessary files
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/share/brotli")

# Skip copying tools in debug builds - the brotli executable is not built in debug mode
# Only copy tools if VCPKG_BUILD_TYPE is not set to debug
if(NOT VCPKG_BUILD_TYPE STREQUAL "debug")
    vcpkg_copy_tools(TOOL_NAMES brotli AUTO_CLEAN)
endif()

# Handle copyright
vcpkg_install_copyright("${SOURCE_PATH}/LICENSE")

