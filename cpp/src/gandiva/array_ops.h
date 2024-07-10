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

#include <cstdint>

#include "gandiva/visibility.h"

namespace llvm {
class VectorType;
}

/// Array functions that can be accessed from LLVM.
extern "C" {

GANDIVA_EXPORT
bool array_int32_contains_int32(int64_t context_ptr, const int32_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int32_t contains_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_buf);
GANDIVA_EXPORT
bool array_int64_contains_int64(int64_t context_ptr, const int64_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int64_t contains_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_buf);

GANDIVA_EXPORT
bool array_float32_contains_float32(int64_t context_ptr, const float* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              float contains_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_buf);

GANDIVA_EXPORT
bool array_float64_contains_float64(int64_t context_ptr, const double* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              double contains_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_buf);

GANDIVA_EXPORT
int32_t* array_int32_remove(int64_t context_ptr, const int32_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int32_t remove_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr);

GANDIVA_EXPORT
int64_t* array_int64_remove(int64_t context_ptr, const int64_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int64_t remove_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr);

GANDIVA_EXPORT
float* array_float32_remove(int64_t context_ptr, const float* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              float remove_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr);

GANDIVA_EXPORT
double* array_float64_remove(int64_t context_ptr, const double* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              double remove_data, bool entry_validWhat, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr);
GANDIVA_EXPORT
char* array_int32_to_string(int64_t context_ptr, const int32_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int32_t contains_data, bool contains_data_valid, 
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, id_t* out_len);
GANDIVA_EXPORT
char* array_int64_to_string(int64_t context_ptr, const int64_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, id_t* out_len);
GANDIVA_EXPORT
char* array_float32_to_string(int64_t context_ptr, const float* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, id_t* out_len);
GANDIVA_EXPORT
char* array_float64_to_string(int64_t context_ptr, const double* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, id_t* out_len);
}
