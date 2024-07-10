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

#include "gandiva/array_ops.h"

#include <bitset>
#include <iostream>
#include <string>

#include "arrow/util/value_parsing.h"

#include "gandiva/gdv_function_stubs.h"
#include "gandiva/engine.h"
#include "gandiva/exported_funcs.h"

/// Stub functions that can be accessed from LLVM or the pre-compiled library.

template <typename Type>
Type* array_remove_template(int64_t context_ptr, const Type* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              Type remove_data, bool remove_data_valid,
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr)
{
  std::vector<Type> newInts;

  const int32_t* entry_validityAdjusted = entry_validity - (loop_var );
  int64_t validityBitIndex = 0;
  //The validity index already has the current row length added to it, so decrement.
  validityBitIndex = validity_index_var - entry_len;
  std::vector<bool> outValid;
  for (int i = 0; i < entry_len; i++) {
    Type entry_item = *(entry_buf + i);
    if (remove_data_valid && entry_item == remove_data) {
      //Do not add the item to remove.
      } else if (!arrow::bit_util::GetBit(reinterpret_cast<const uint8_t*>(entry_validityAdjusted), validityBitIndex + i)) {
      outValid.push_back(false);
      newInts.push_back(0);
    } else {
      outValid.push_back(true);
      newInts.push_back(entry_item);
    }
  }

  *out_len = (int)newInts.size();

  //Since this function can remove values we don't know the length ahead of time.
  //A fast way to compute Math.ceil(input / 8.0).
  int validByteSize = (unsigned int)((*out_len) + 7) >> 3;

  uint8_t* validRet = gdv_fn_context_arena_malloc(context_ptr, validByteSize);
  for (size_t i = 0; i < outValid.size(); i++) {
    arrow::bit_util::SetBitTo(validRet, i, outValid[i]);
  }

  int32_t outBufferLength = (int)*out_len * sizeof(Type);
  //length is number of items, but buffers must account for byte size.
  uint8_t* ret = gdv_fn_context_arena_malloc(context_ptr, outBufferLength);
  memcpy(ret, newInts.data(), outBufferLength);
  *valid_row = true;

  //Return null if the input array is null or the data to remove is null.
  if (!combined_row_validity || !remove_data_valid) {
    *out_len = 0;
    *valid_row = false;  //this one is what works for the top level validity.
  }

  *valid_ptr = reinterpret_cast<int32_t*>(validRet);
  return reinterpret_cast<Type*>(ret);
}

template <typename Type>
bool array_contains_template(const Type* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              Type contains_data, bool contains_data_valid,
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row) {
  if (!combined_row_validity || !contains_data_valid) {
    *valid_row = false;
    return false;
  }
  *valid_row = true;

  const int32_t* entry_validityAdjusted = entry_validity - (loop_var );
  int64_t validityBitIndex = validity_index_var - entry_len;
  
  bool found_null_in_data = false;
  for (int i = 0; i < entry_len; i++) {
    if (!arrow::bit_util::GetBit(reinterpret_cast<const uint8_t*>(entry_validityAdjusted), validityBitIndex + i)) {
      found_null_in_data = true;
      continue;
    }
    Type entry_item = *(entry_buf + i);
    if (contains_data_valid && entry_item == contains_data) {
      return true;
    }
  }
  //If there is null in the input and the item is not found the result is null.
  if (found_null_in_data) {
    *valid_row = false;
  }
  return false;
}

template <typename Type>
char* array_to_string_template(const Type* entry_buf,
                   int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                   const char* delimiter, gdv_int32 delimiter_len, int64_t loop_var, int64_t validity_index_var,
                   bool* valid_row, gdv_int32* out_len) {
  bool first_element = true;

  const int32_t* entry_validityAdjusted = entry_validity - entry_len;
  int64_t validityBitIndex = -entry_len;
  std::string result; // Initialize the result variable as an empty string

  for (int i = 0; i < entry_len; i++) {
    if (!arrow::bit_util::GetBit(reinterpret_cast<const uint8_t*>(entry_validityAdjusted), validityBitIndex + i)) {
      continue;
    }

    if (!first_element) {
      result += delimiter;
    }

    Type entry_item = *(entry_buf + i);
    result += std::to_string(entry_item);

    first_element = false;
  }
  *valid_row = true;
  char* result_char = reinterpret_cast<char*>(gdv_fn_context_arena_malloc(context, *out_len));
  std::strcpy(result_char, result.c_str());
  return result_char;
}

extern "C" {

bool array_int32_contains_int32(int64_t context_ptr, const int32_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int32_t contains_data, bool contains_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row) {
  return array_contains_template<int32_t>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, contains_data, contains_data_valid,
                              loop_var, validity_index_var, valid_row);
}

bool array_int64_contains_int64(int64_t context_ptr, const int64_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int64_t contains_data, bool contains_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row) {
  return array_contains_template<int64_t>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, contains_data, contains_data_valid,
                              loop_var, validity_index_var, valid_row);
}

bool array_float32_contains_float32(int64_t context_ptr, const float* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              float contains_data, bool contains_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row) {
  return array_contains_template<float>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, contains_data, contains_data_valid,
                              loop_var, validity_index_var, valid_row);
}

bool array_float64_contains_float64(int64_t context_ptr, const double* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              double contains_data, bool contains_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row) {
  return array_contains_template<double>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, contains_data, contains_data_valid,
                              loop_var, validity_index_var, valid_row);
}



int32_t* array_int32_remove(int64_t context_ptr, const int32_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int32_t remove_data, bool remove_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr) {
  return array_remove_template<int32_t>(context_ptr, entry_buf,
                               entry_len, entry_validity, combined_row_validity,
                              remove_data, remove_data_valid,
                              loop_var, validity_index_var,
                              valid_row, out_len, valid_ptr);
}

int64_t* array_int64_remove(int64_t context_ptr, const int64_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int64_t remove_data, bool remove_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr){
  return array_remove_template<int64_t>(context_ptr, entry_buf,
                               entry_len, entry_validity, combined_row_validity,
                              remove_data, remove_data_valid,
                              loop_var, validity_index_var,
                              valid_row, out_len, valid_ptr);
}

float* array_float32_remove(int64_t context_ptr, const float* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              float remove_data, bool remove_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr){
  return array_remove_template<float>(context_ptr, entry_buf,
                               entry_len, entry_validity, combined_row_validity,
                              remove_data, remove_data_valid,
                              loop_var, validity_index_var,
                              valid_row, out_len, valid_ptr);
}


double* array_float64_remove(int64_t context_ptr, const double* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              double remove_data, bool remove_data_valid, 
                              int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len, int32_t** valid_ptr){
  return array_remove_template<double>(context_ptr, entry_buf,
                               entry_len, entry_validity, combined_row_validity,
                              remove_data, remove_data_valid,
                              loop_var, validity_index_var,
                              valid_row, out_len, valid_ptr);
}

char* array_int32_to_string(int64_t context_ptr, const int32_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              int32_t contains_data, bool contains_data_valid, 
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len) {
  return array_to_string_template<int32_t>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, delimiter, delimiter_len, loop_var, validity_index_var, valid_row, out_len);
}

char* array_int64_to_string(int64_t context_ptr, const int64_t* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, id_t* out_len) {
  return array_to_string_template<int64_t>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, delimiter, delimiter_len, loop_var, validity_index_var, valid_row, out_len);
}

char* array_float32_to_string(int64_t context_ptr, const float* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              const char* delimiter,int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t** out_len) {
  return array_to_string_template<float>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, delimiter, delimiter_len, loop_var, validity_index_var, valid_row, out_len);
}

char* array_float64_to_string(int64_t context_ptr, const double* entry_buf,
                              int32_t entry_len, const int32_t* entry_validity, bool combined_row_validity,
                              const char* delimiter, int32_t delimiter_len, int64_t loop_var, int64_t validity_index_var,
                              bool* valid_row, int32_t* out_len) {
  return array_to_string_template<double>(entry_buf, entry_len, entry_validity, 
                              combined_row_validity, delimiter, delimiter_len, loop_var, validity_index_var, valid_row, out_len);
}
}

namespace gandiva {
arrow::Status ExportedArrayFunctions::AddMappings(Engine* engine) const {
  std::vector<llvm::Type*> args;
  auto types = engine->types();

  args = {types->i64_type(),      // int64_t execution_context
          types->i64_ptr_type(),   // int8_t* data ptr
          types->i32_type(),      // int32_t data length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i32_type(),     // int32_t value to check for
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row
          };

  engine->AddGlobalMappingForFunc("array_int32_contains_int32",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(array_int32_contains_int32));

  args = {types->i64_type(),      // int64_t execution_context
          types->i64_ptr_type(),   // int8_t* data ptr
          types->i32_type(),      // int32_t data length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i64_type(),     // int32_t value to check for
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row
          };

  engine->AddGlobalMappingForFunc("array_int64_contains_int64",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(array_int64_contains_int64));

  args = {types->i64_type(),      // int64_t execution_context
          types->float_ptr_type(),   // int8_t* data ptr
          types->i32_type(),      // int32_t data length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->float_type(),     // int32_t value to check for
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row
          };

  engine->AddGlobalMappingForFunc("array_float32_contains_float32",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(array_float32_contains_float32));

  args = {types->i64_type(),      // int64_t execution_context
          types->double_ptr_type(),   // int8_t* data ptr
          types->i32_type(),      // int32_t data length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->double_type(),     // int32_t value to check for
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row
          };

  engine->AddGlobalMappingForFunc("array_float64_contains_float64",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(array_float64_contains_float64));
  //Array remove.
  args = {types->i64_type(),      // int64_t execution_context
          types->i32_ptr_type(),   // int8_t* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i32_type(),      //value to remove from input
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type(),   //output validity for the row
          types->i32_ptr_type(),   // output array length
          types->i32_ptr_type()  //output pointer to new validity buffer
          
        };
  engine->AddGlobalMappingForFunc("array_int32_remove",
                                  types->i32_ptr_type(), args,
                                  reinterpret_cast<void*>(array_int32_remove));

  args = {types->i64_type(),      // int64_t execution_context
          types->i64_ptr_type(),   // int8_t* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i64_type(),      //value to remove from input
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type(),   //output validity for the row
          types->i32_ptr_type(),   // output array length
          types->i32_ptr_type()  //output pointer to new validity buffer
          
        };

  engine->AddGlobalMappingForFunc("array_int64_remove",
                                  types->i64_ptr_type(), args,
                                  reinterpret_cast<void*>(array_int64_remove));

  args = {types->i64_type(),      // int64_t execution_context
          types->float_ptr_type(),   // float* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->float_type(),      //value to remove from input
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type(),   //output validity for the row
          types->i32_ptr_type(),   // output array length
          types->i32_ptr_type()  //output pointer to new validity buffer
          
        };

  engine->AddGlobalMappingForFunc("array_float32_remove",
                                  types->float_ptr_type(), args,
                                  reinterpret_cast<void*>(array_float32_remove));

  args = {types->i64_type(),      // int64_t execution_context
          types->double_ptr_type(),   // int8_t* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->double_type(),      //value to remove from input
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type(),   //output validity for the row
          types->i32_ptr_type(),   // output array length
          types->i32_ptr_type()  //output pointer to new validity buffer
          
        };

  engine->AddGlobalMappingForFunc("array_float64_remove",
                                  types->double_ptr_type(), args,
                                  reinterpret_cast<void*>(array_float64_remove));


  //Array to string.
  args = {types->i64_type(),      // int64_t execution_context
          types->i32_ptr_type(),   // int8_t* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i8_ptr_type(),
          types->i32_ptr_type()   //delimiter length
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row   
          types->i32_ptr_type(),   // output length         //value to for delimiter
        };
  engine->AddGlobalMappingForFunc("array_int32_to_string",
                                  types->i32_ptr_type(), args,
                                  reinterpret_cast<void*>(array_int32_to_string));

  args = {types->i64_type(),      // int64_t execution_context
          types->i64_ptr_type(),   // int8_t* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i8_ptr_type(),      //value to for delimiter
          types->i32_ptr_type(),   //delimiter length
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row   
          types->i32_ptr_type(),   // output length         
        };

  engine->AddGlobalMappingForFunc("array_int64_to_string",
                                  types->i64_ptr_type(), args,
                                  reinterpret_cast<void*>(array_int64_to_string));

  args = {types->i64_type(),      // int64_t execution_context
          types->float_ptr_type(),   // float* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i8_ptr_type(),      //value to for delimiter
          types->i32_ptr_type(),   //delimiter length
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row   
          types->i32_ptr_type(),   // output length   
        };

  engine->AddGlobalMappingForFunc("array_float32_to_string",
                                  types->float_ptr_type(), args,
                                  reinterpret_cast<void*>(array_float32_to_string));

  args = {types->i64_type(),      // int64_t execution_context
          types->double_ptr_type(),   // int8_t* input data ptr
          types->i32_type(),      // int32_t  input length
          types->i32_ptr_type(),   // input validity buffer
          types->i1_type(),   // bool input row validity
          types->i8_ptr_type(),      //value to for delimiter
          types->i32_ptr_type(),   //delimiter length
          types->i1_type(),   // bool validity --Needed?
          types->i64_type(),      //in loop var  --Needed?
          types->i64_type(),      //in validity_index_var index into the valdity vector for the current row.
          types->i1_ptr_type()   //output validity for the row   
          types->i32_ptr_type(),   // output length         
        };

  engine->AddGlobalMappingForFunc("array_float64_to_string",
                                  types->double_ptr_type(), args,
                                  reinterpret_cast<void*>(array_float64_to_string));                                
  return arrow::Status::OK();
}
}  // namespace gandiva
