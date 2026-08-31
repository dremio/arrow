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

#include "gandiva/gdv_function_stubs.h"

#include <utf8proc.h>

#include <algorithm>
#include <boost/crc.hpp>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

#include "arrow/util/base64.h"
#include "arrow/util/bit_util.h"
#include "arrow/util/double_conversion_internal.h"
#include "arrow/util/value_parsing.h"

#include "gandiva/encrypt_utils_ecb.h"
#include "gandiva/encrypt_utils_cbc.h"
#include "gandiva/encrypt_mode_dispatcher.h"
#include "gandiva/engine.h"
#include "gandiva/exported_funcs.h"
#include "gandiva/in_holder.h"
#include "gandiva/interval_holder.h"
#include "gandiva/random_generator_holder.h"
#include "gandiva/to_date_holder.h"

/// Stub functions that can be accessed from LLVM or the pre-compiled library.

extern "C" {

ARROW_SUPPRESS_MISSING_DECLARATIONS_WARNING

static char mask_array[256] = {
    (char)0,  (char)1,  (char)2,  (char)3,   (char)4,   (char)5,   (char)6,   (char)7,
    (char)8,  (char)9,  (char)10, (char)11,  (char)12,  (char)13,  (char)14,  (char)15,
    (char)16, (char)17, (char)18, (char)19,  (char)20,  (char)21,  (char)22,  (char)23,
    (char)24, (char)25, (char)26, (char)27,  (char)28,  (char)29,  (char)30,  (char)31,
    (char)32, (char)33, (char)34, (char)35,  (char)36,  (char)37,  (char)38,  (char)39,
    (char)40, (char)41, (char)42, (char)43,  (char)44,  (char)45,  (char)46,  (char)47,
    'n',      'n',      'n',      'n',       'n',       'n',       'n',       'n',
    'n',      'n',      (char)58, (char)59,  (char)60,  (char)61,  (char)62,  (char)63,
    (char)64, 'X',      'X',      'X',       'X',       'X',       'X',       'X',
    'X',      'X',      'X',      'X',       'X',       'X',       'X',       'X',
    'X',      'X',      'X',      'X',       'X',       'X',       'X',       'X',
    'X',      'X',      'X',      (char)91,  (char)92,  (char)93,  (char)94,  (char)95,
    (char)96, 'x',      'x',      'x',       'x',       'x',       'x',       'x',
    'x',      'x',      'x',      'x',       'x',       'x',       'x',       'x',
    'x',      'x',      'x',      'x',       'x',       'x',       'x',       'x',
    'x',      'x',      'x',      (char)123, (char)124, (char)125, (char)126, (char)127};

double gdv_fn_random(int64_t ptr) {
  gandiva::RandomGeneratorHolder* holder =
      reinterpret_cast<gandiva::RandomGeneratorHolder*>(ptr);
  return (*holder)();
}

double gdv_fn_random_with_seed(int64_t ptr, int32_t /*seed*/, bool /*seed_validity*/) {
  gandiva::RandomGeneratorHolder* holder =
      reinterpret_cast<gandiva::RandomGeneratorHolder*>(ptr);
  return (*holder)();
}

int32_t gdv_fn_rand_integer(int64_t ptr) {
  gandiva::RandomIntegerGeneratorHolder* holder =
      reinterpret_cast<gandiva::RandomIntegerGeneratorHolder*>(ptr);
  return (*holder)();
}

int32_t gdv_fn_rand_integer_with_range(int64_t ptr, int32_t /*range*/,
                                       bool /*range_validity*/) {
  gandiva::RandomIntegerGeneratorHolder* holder =
      reinterpret_cast<gandiva::RandomIntegerGeneratorHolder*>(ptr);
  return (*holder)();
}

int32_t gdv_fn_rand_integer_with_min_max(int64_t ptr, int32_t /*min*/,
                                         bool /*min_validity*/, int32_t /*max*/,
                                         bool /*max_validity*/) {
  gandiva::RandomIntegerGeneratorHolder* holder =
      reinterpret_cast<gandiva::RandomIntegerGeneratorHolder*>(ptr);
  return (*holder)();
}

bool gdv_fn_in_expr_lookup_int32(int64_t ptr, int32_t value, bool in_validity) {
  if (!in_validity) {
    return false;
  }
  gandiva::InHolder<int32_t>* holder = reinterpret_cast<gandiva::InHolder<int32_t>*>(ptr);
  return holder->HasValue(value);
}

bool gdv_fn_in_expr_lookup_int64(int64_t ptr, int64_t value, bool in_validity) {
  if (!in_validity) {
    return false;
  }
  gandiva::InHolder<int64_t>* holder = reinterpret_cast<gandiva::InHolder<int64_t>*>(ptr);
  return holder->HasValue(value);
}

bool gdv_fn_in_expr_lookup_decimal(int64_t ptr, int64_t value_high, int64_t value_low,
                                   int32_t precision, int32_t scale, bool in_validity) {
  if (!in_validity) {
    return false;
  }
  gandiva::DecimalScalar128 value(value_high, value_low, precision, scale);
  gandiva::InHolder<gandiva::DecimalScalar128>* holder =
      reinterpret_cast<gandiva::InHolder<gandiva::DecimalScalar128>*>(ptr);
  return holder->HasValue(value);
}

bool gdv_fn_in_expr_lookup_float(int64_t ptr, float value, bool in_validity) {
  if (!in_validity) {
    return false;
  }
  gandiva::InHolder<float>* holder = reinterpret_cast<gandiva::InHolder<float>*>(ptr);
  return holder->HasValue(value);
}

bool gdv_fn_in_expr_lookup_double(int64_t ptr, double value, bool in_validity) {
  if (!in_validity) {
    return false;
  }
  gandiva::InHolder<double>* holder = reinterpret_cast<gandiva::InHolder<double>*>(ptr);
  return holder->HasValue(value);
}

bool gdv_fn_in_expr_lookup_utf8(int64_t ptr, const char* data, int data_len,
                                bool in_validity) {
  if (!in_validity) {
    return false;
  }
  gandiva::InHolder<std::string>* holder =
      reinterpret_cast<gandiva::InHolder<std::string>*>(ptr);
  return holder->HasValue(std::string_view(data, data_len));
}

int32_t gdv_fn_populate_varlen_vector(int64_t context_ptr, int8_t* data_ptr,
                                      int32_t* offsets, int64_t slot,
                                      const char* entry_buf, int32_t entry_len) {
  auto buffer = reinterpret_cast<arrow::ResizableBuffer*>(data_ptr);
  int32_t offset = static_cast<int32_t>(buffer->size());

  auto new_size = offset + entry_len;
  // preallocation, double the size to amortize costs
  if (buffer->capacity() < new_size) {
    auto status =
        buffer->Reserve(std::max(buffer->capacity() * 2, static_cast<int64_t>(new_size)));
    if (!status.ok()) {
      auto context = reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);

      context->set_error_msg(status.message().c_str());
      return -1;
    }
  }

  // This only sets the size in the buffer due to preallocation.
  auto status = buffer->Resize(new_size, false /*shrink*/);
  if (!status.ok()) {
    gandiva::ExecutionContext* context =
        reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);

    context->set_error_msg(status.message().c_str());
    return -1;
  }

  // append the new entry.
  memcpy(buffer->mutable_data() + offset, entry_buf, entry_len);

  // update offsets buffer.
  offsets[slot] = offset;
  offsets[slot + 1] = offset + entry_len;
  return 0;
}

#define CRC_FUNCTION(TYPE)                                                          \
  GANDIVA_EXPORT                                                                    \
  int64_t gdv_fn_crc_32_##TYPE(int64_t ctx, const char* input, int32_t input_len) { \
    if (input_len < 0) {                                                            \
      char err_msg[96];                                                             \
      snprintf(err_msg, sizeof(err_msg),                                            \
               "CRC32: Input length can't be negative, got %d", input_len);         \
      gdv_fn_context_set_error_msg(ctx, err_msg);                                   \
      return 0;                                                                     \
    }                                                                               \
    boost::crc_32_type result;                                                      \
    result.process_bytes(input, input_len);                                         \
    return result.checksum();                                                       \
  }
CRC_FUNCTION(utf8)
CRC_FUNCTION(binary)

int32_t gdv_fn_dec_from_string(int64_t context, const char* in, int32_t in_length,
                               int32_t* precision_from_str, int32_t* scale_from_str,
                               int64_t* dec_high_from_str, uint64_t* dec_low_from_str) {
  arrow::Decimal128 dec;
  auto status = arrow::Decimal128::FromString(std::string(in, in_length), &dec,
                                              precision_from_str, scale_from_str);
  if (!status.ok()) {
    gdv_fn_context_set_error_msg(context, status.message().data());
    return -1;
  }
  *dec_high_from_str = dec.high_bits();
  *dec_low_from_str = dec.low_bits();
  return 0;
}

char* gdv_fn_dec_to_string(int64_t context, int64_t x_high, uint64_t x_low,
                           int32_t x_scale, int32_t* dec_str_len) {
  arrow::Decimal128 dec(arrow::BasicDecimal128(x_high, x_low));
  std::string dec_str = dec.ToString(x_scale);
  auto dec_str_length = static_cast<int32_t>(dec_str.length());
  char* ret =
      reinterpret_cast<char*>(gdv_fn_context_arena_malloc(context, dec_str_length));
  if (ret == nullptr) {
    std::string err_msg = "Could not allocate memory for string: " + dec_str;
    gdv_fn_context_set_error_msg(context, err_msg.data());
    // Report zero length so a caller can never combine a positive length with the
    // null buffer (the original bug: memcpy(dst, nullptr, positive_len) -> SIGSEGV).
    *dec_str_len = 0;
    return nullptr;
  }
  *dec_str_len = dec_str_length;
  memcpy(ret, dec_str.data(), dec_str_length);
  return ret;
}

GANDIVA_EXPORT
const char* gdv_fn_base64_encode_binary(int64_t context, const char* in, int32_t in_len,
                                        int32_t* out_len) {
  if (in_len < 0) {
    char err_msg[96];
    snprintf(err_msg, sizeof(err_msg),
             "BASE64: input length must be non-negative, got %d", in_len);
    gdv_fn_context_set_error_msg(context, err_msg);
    *out_len = 0;
    return "";
  }
  if (in_len == 0) {
    *out_len = 0;
    return "";
  }
  // use arrow method to encode base64 string
  std::string encoded_str = arrow::util::base64_encode(std::string_view(in, in_len));
  *out_len = static_cast<int32_t>(encoded_str.length());
  // allocate memory for response
  char* ret = reinterpret_cast<char*>(
      gdv_fn_context_arena_malloc(context, static_cast<int32_t>(*out_len)));
  if (ret == nullptr) {
    gdv_fn_context_set_error_msg(context, "Could not allocate memory");
    *out_len = 0;
    return "";
  }
  memcpy(ret, encoded_str.data(), *out_len);
  return ret;
}

GANDIVA_EXPORT
const char* gdv_fn_base64_decode_utf8(int64_t context, const char* in, int32_t in_len,
                                      int32_t* out_len) {
  if (in_len < 0) {
    char err_msg[96];
    snprintf(err_msg, sizeof(err_msg),
             "UNBASE64: input length must be non-negative, got %d", in_len);
    gdv_fn_context_set_error_msg(context, err_msg);
    *out_len = 0;
    return "";
  }
  if (in_len == 0) {
    *out_len = 0;
    return "";
  }
  // use arrow method to decode base64 string
  std::string decoded_str = arrow::util::base64_decode(std::string_view(in, in_len));
  *out_len = static_cast<int32_t>(decoded_str.length());
  // allocate memory for response
  char* ret = reinterpret_cast<char*>(
      gdv_fn_context_arena_malloc(context, static_cast<int32_t>(*out_len)));
  if (ret == nullptr) {
    gdv_fn_context_set_error_msg(context, "Could not allocate memory");
    *out_len = 0;
    return "";
  }
  memcpy(ret, decoded_str.data(), *out_len);
  return ret;
}

#define CAST_NUMERIC_FROM_VARLEN_TYPES(OUT_TYPE, ARROW_TYPE, TYPE_NAME, INNER_TYPE)  \
  GANDIVA_EXPORT                                                                     \
  OUT_TYPE gdv_fn_cast##TYPE_NAME##_##INNER_TYPE(int64_t context, const char* data,  \
                                                 int32_t len) {                      \
    OUT_TYPE val = 0;                                                                \
    /* trim leading and trailing spaces */                                           \
    int32_t trimmed_len;                                                             \
    int32_t start = 0, end = len - 1;                                                \
    while (start <= end && data[start] == ' ') {                                     \
      ++start;                                                                       \
    }                                                                                \
    while (end >= start && data[end] == ' ') {                                       \
      --end;                                                                         \
    }                                                                                \
    trimmed_len = end - start + 1;                                                   \
    const char* trimmed_data = data + start;                                         \
    if (!arrow::internal::ParseValue<ARROW_TYPE>(trimmed_data, trimmed_len, &val)) { \
      std::string err =                                                              \
          "Failed to cast the string " + std::string(data, len) + " to " #OUT_TYPE;  \
      gdv_fn_context_set_error_msg(context, err.c_str());                            \
    }                                                                                \
    return val;                                                                      \
  }

#define CAST_NUMERIC_FROM_STRING(OUT_TYPE, ARROW_TYPE, TYPE_NAME) \
  CAST_NUMERIC_FROM_VARLEN_TYPES(OUT_TYPE, ARROW_TYPE, TYPE_NAME, utf8)

CAST_NUMERIC_FROM_STRING(int32_t, arrow::Int32Type, INT)
CAST_NUMERIC_FROM_STRING(int64_t, arrow::Int64Type, BIGINT)
CAST_NUMERIC_FROM_STRING(float, arrow::FloatType, FLOAT4)
CAST_NUMERIC_FROM_STRING(double, arrow::DoubleType, FLOAT8)

#undef CAST_NUMERIC_FROM_STRING

#define CAST_NUMERIC_FROM_VARBINARY(OUT_TYPE, ARROW_TYPE, TYPE_NAME) \
  CAST_NUMERIC_FROM_VARLEN_TYPES(OUT_TYPE, ARROW_TYPE, TYPE_NAME, varbinary)

CAST_NUMERIC_FROM_VARBINARY(int32_t, arrow::Int32Type, INT)
CAST_NUMERIC_FROM_VARBINARY(int64_t, arrow::Int64Type, BIGINT)
CAST_NUMERIC_FROM_VARBINARY(float, arrow::FloatType, FLOAT4)
CAST_NUMERIC_FROM_VARBINARY(double, arrow::DoubleType, FLOAT8)

#undef CAST_NUMERIC_STRING

#undef GDV_FN_CAST_VARCHAR_INTEGER
#undef GDV_FN_CAST_VARCHAR_REAL



GANDIVA_EXPORT
const char* gdv_mask_first_n_utf8_int32(int64_t context, const char* data,
                                        int32_t data_len, int32_t n_to_mask,
                                        int32_t* out_len) {
  if (data_len <= 0) {
    *out_len = 0;
    return nullptr;
  }

  if (n_to_mask > data_len) {
    n_to_mask = data_len;
  }

  *out_len = data_len;

  if (n_to_mask <= 0) {
    return data;
  }

  char* out = reinterpret_cast<char*>(gdv_fn_context_arena_malloc(context, *out_len));
  if (out == nullptr) {
    gdv_fn_context_set_error_msg(context, "Could not allocate memory for output string");
    *out_len = 0;
    return nullptr;
  }

  int bytes_masked;
  for (bytes_masked = 0; bytes_masked < n_to_mask; bytes_masked++) {
    unsigned char char_single_byte = data[bytes_masked];
    if (char_single_byte > 127) {
      // found a multi-byte utf-8 char
      break;
    }
    out[bytes_masked] = mask_array[char_single_byte];
  }

  int chars_masked = bytes_masked;
  int out_idx = bytes_masked;

  // Handle multibyte utf8 characters
  utf8proc_int32_t utf8_char;
  while ((chars_masked < n_to_mask) && (bytes_masked < data_len)) {
    auto char_len =
        utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(data + bytes_masked),
                         data_len, &utf8_char);

    if (char_len < 0) {
      gdv_fn_context_set_error_msg(context, utf8proc_errmsg(char_len));
      *out_len = 0;
      return nullptr;
    }

    // Only uppercase letters (Lu), lowercase letters (Ll) and decimal digits (Nd)
    // are masked, per the Hive MASK specification (A-Z : X, a-z : x, 0-9 : n).
    // Every other Unicode general category passes through unchanged -- including
    // titlecase letters (Lt), other letters (Lo, e.g. CJK and Hangul), letter
    // numbers (Nl, e.g. Roman numerals) and other numbers (No, e.g. fractions),
    // which have no case or decimal-digit meaning to map onto X/x/n. This matches
    // the ASCII mask_array above and the `mask` function below.
    switch (utf8proc_category(utf8_char)) {
      case UTF8PROC_CATEGORY_LU:
        out[out_idx] = 'X';
        out_idx++;
        break;
      case UTF8PROC_CATEGORY_LL:
        out[out_idx] = 'x';
        out_idx++;
        break;
      case UTF8PROC_CATEGORY_ND:
        out[out_idx] = 'n';
        out_idx++;
        break;
      default:
        memcpy(out + out_idx, data + bytes_masked, char_len);
        out_idx += static_cast<int>(char_len);
        break;
    }
    bytes_masked += static_cast<int>(char_len);
    chars_masked++;
  }

  // Correct the out_len after masking multibyte characters with single byte characters
  *out_len = *out_len - (bytes_masked - out_idx);

  if (bytes_masked < data_len) {
    memcpy(out + out_idx, data + bytes_masked, data_len - bytes_masked);
  }

  return out;
}

GANDIVA_EXPORT
const char* gdv_mask_last_n_utf8_int32(int64_t context, const char* data,
                                       int32_t data_len, int32_t n_to_mask,
                                       int32_t* out_len) {
  if (data_len <= 0) {
    *out_len = 0;
    return nullptr;
  }

  if (n_to_mask > data_len) {
    n_to_mask = data_len;
  }

  *out_len = data_len;

  if (n_to_mask <= 0) {
    return data;
  }

  char* out = reinterpret_cast<char*>(gdv_fn_context_arena_malloc(context, *out_len));
  if (out == nullptr) {
    gdv_fn_context_set_error_msg(context, "Could not allocate memory for output string");
    *out_len = 0;
    return nullptr;
  }

  bool has_multi_byte = false;
  for (int i = 0; i < data_len; i++) {
    unsigned char char_single_byte = data[i];
    if (char_single_byte > 127) {
      // found a multi-byte utf-8 char
      has_multi_byte = true;
      break;
    }
  }

  if (!has_multi_byte) {
    int start_idx = data_len - n_to_mask;
    memcpy(out, data, start_idx);
    for (int i = start_idx; i < data_len; ++i) {
      unsigned char char_single_byte = data[i];
      out[i] = mask_array[char_single_byte];
    }
    *out_len = data_len;
    return out;
  }

  utf8proc_int32_t utf8_char_buffer;
  int num_of_chars = static_cast<int>(
      utf8proc_decompose(reinterpret_cast<const utf8proc_uint8_t*>(data), data_len,
                         &utf8_char_buffer, 1, UTF8PROC_STABLE));

  if (num_of_chars < 0) {
    gdv_fn_context_set_error_msg(context, utf8proc_errmsg(num_of_chars));
    *out_len = 0;
    return nullptr;
  }

  utf8proc_int32_t utf8_char;
  int chars_counter = 0;
  int bytes_read = 0;
  while ((bytes_read < data_len) && (chars_counter < (num_of_chars - n_to_mask))) {
    auto char_len =
        utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(data + bytes_read),
                         data_len, &utf8_char);
    chars_counter++;
    bytes_read += static_cast<int>(char_len);
  }

  int out_idx = bytes_read;
  int offset_idx = bytes_read;

  // Populate the first chars, that are not masked
  memcpy(out, data, offset_idx);

  while (bytes_read < data_len) {
    auto char_len =
        utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(data + bytes_read),
                         data_len, &utf8_char);
    // Only Lu / Ll / Nd are masked; see gdv_mask_first_n_utf8_int32 above.
    switch (utf8proc_category(utf8_char)) {
      case UTF8PROC_CATEGORY_LU:
        out[out_idx] = 'X';
        out_idx++;
        break;
      case UTF8PROC_CATEGORY_LL:
        out[out_idx] = 'x';
        out_idx++;
        break;
      case UTF8PROC_CATEGORY_ND:
        out[out_idx] = 'n';
        out_idx++;
        break;
      default:
        memcpy(out + out_idx, data + bytes_read, char_len);
        out_idx += static_cast<int>(char_len);
        break;
    }
    bytes_read += static_cast<int>(char_len);
  }

  *out_len = out_idx;

  return out;
}

/// Shared implementation of the mask() overloads.
///
/// \p other is the replacement for every character that is neither an uppercase letter,
/// a lowercase letter nor a decimal digit. A null \p other leaves those characters
/// unchanged, which is the Hive default and what the one- to four-argument overloads
/// pass; the five-argument overload passes the caller's replacement instead.
static const char* mask_impl(int64_t context, const char* data, int32_t data_len,
                             const char* upper, int32_t upper_length, const char* lower,
                             int32_t lower_length, const char* num, int32_t num_length,
                             const char* other, int32_t other_length, int32_t* out_len) {
  if (data_len <= 0) {
    *out_len = 0;
    return nullptr;
  }

  // An empty replacement argument means "use the default for this class", matching
  // Hive's GenericUDFMaskBase, rather than deleting the character. The default for
  // `other` is to leave the character alone, so an empty `other` becomes a null one.
  //
  // This also keeps every replacement length >= 1, which the output size bound
  // below depends on: with max_repl >= 1 and every input character at least one
  // byte wide, max_repl * data_len >= sum(max(max_repl, char_len_i)), so the
  // allocation covers both replaced and passed-through characters. If an empty
  // argument reached the bound, max_repl would be 0 and the allocation would be
  // too small for the characters that pass through unmasked.
  if (upper_length <= 0) {
    upper = "X";
    upper_length = 1;
  }
  if (lower_length <= 0) {
    lower = "x";
    lower_length = 1;
  }
  if (num_length <= 0) {
    num = "n";
    num_length = 1;
  }
  if (other != nullptr && other_length <= 0) {
    other = nullptr;
  }

  // Every replacement is at least one byte and so is every input character, so
  // max_repl_len * data_len bounds the output. Both factors are user-supplied
  // expressions, so compute in 64 bits: in 32-bit math a long replacement over a long
  // input wraps negative, and SimpleArena::Allocate would then wind its cursor back by
  // that amount and hand out a pointer the masking loop writes past.
  const int32_t max_repl_len =
      std::max(other == nullptr ? 0 : other_length,
               std::max(upper_length, std::max(lower_length, num_length)));
  const int64_t max_length = static_cast<int64_t>(max_repl_len) * data_len;
  if (max_length > std::numeric_limits<int32_t>::max()) {
    gdv_fn_context_set_error_msg(context, "Mask output would exceed the maximum size");
    *out_len = 0;
    return nullptr;
  }

  char* out = reinterpret_cast<char*>(
      gdv_fn_context_arena_malloc(context, static_cast<int32_t>(max_length)));
  if (out == nullptr) {
    gdv_fn_context_set_error_msg(context, "Could not allocate memory for output string");
    *out_len = 0;
    return nullptr;
  }

  bool has_multi_byte = false;
  for (int i = 0; i < data_len; i++) {
    unsigned char char_single_byte = data[i];
    if (char_single_byte > 127) {
      // found a multi-byte utf-8 char
      has_multi_byte = true;
      break;
    }
  }

  if (!has_multi_byte) {
    int out_index = 0;
    for (int i = 0; i < data_len; ++i) {
      unsigned char char_single_byte = data[i];
      if (char_single_byte >= 'A' && char_single_byte <= 'Z') {
        memcpy(out + out_index, upper, upper_length);
        out_index += upper_length;
      } else if (char_single_byte >= 'a' && char_single_byte <= 'z') {
        memcpy(out + out_index, lower, lower_length);
        out_index += lower_length;
      } else if (isdigit(char_single_byte)) {
        memcpy(out + out_index, num, num_length);
        out_index += num_length;
      } else if (other != nullptr) {
        memcpy(out + out_index, other, other_length);
        out_index += other_length;
      } else {
        out[out_index] = char_single_byte;
        out_index++;
      }
    }
    *out_len = out_index;
    return out;
  }

  utf8proc_int32_t utf8_char;
  int bytes_read = 0;
  int32_t out_index = 0;
  while (bytes_read < data_len) {
    auto char_len =
        utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(data + bytes_read),
                         data_len, &utf8_char);
    // Only uppercase letters (Lu), lowercase letters (Ll) and decimal digits (Nd)
    // are masked, per the Hive MASK specification (A-Z : X, a-z : x, 0-9 : n).
    // Every other Unicode general category passes through unchanged -- including
    // titlecase letters (Lt), other letters (Lo, e.g. CJK and Hangul), letter
    // numbers (Nl, e.g. Roman numerals) and other numbers (No, e.g. fractions).
    // Those categories have no case or decimal-digit meaning to map onto X/x/n:
    // scripts in Lo have no case at all, so masking them as "lowercase" would
    // assert something untrue about the input. This matches the ASCII fast path
    // above and gdv_mask_first_n_utf8_int32 / gdv_mask_last_n_utf8_int32.
    switch (utf8proc_category(utf8_char)) {
      case UTF8PROC_CATEGORY_LU:
        memcpy(out + out_index, upper, upper_length);
        out_index += upper_length;
        break;
      case UTF8PROC_CATEGORY_LL:
        memcpy(out + out_index, lower, lower_length);
        out_index += lower_length;
        break;
      case UTF8PROC_CATEGORY_ND:
        memcpy(out + out_index, num, num_length);
        out_index += num_length;
        break;
      default:
        if (other != nullptr) {
          memcpy(out + out_index, other, other_length);
          out_index += other_length;
        } else {
          memcpy(out + out_index, data + bytes_read, char_len);
          out_index += static_cast<int>(char_len);
        }
        break;
    }
    bytes_read += static_cast<int>(char_len);
  }
  *out_len = out_index;
  return out;
}

GANDIVA_EXPORT
const char* mask_utf8_utf8_utf8_utf8_utf8(int64_t context, const char* data,
                                          int32_t data_len, const char* upper,
                                          int32_t upper_length, const char* lower,
                                          int32_t lower_length, const char* num,
                                          int32_t num_length, const char* other,
                                          int32_t other_length, int32_t* out_len) {
  return mask_impl(context, data, data_len, upper, upper_length, lower, lower_length, num,
                   num_length, other, other_length, out_len);
}

GANDIVA_EXPORT
const char* mask_utf8_utf8_utf8_utf8(int64_t context, const char* data, int32_t data_len,
                                     const char* upper, int32_t upper_length,
                                     const char* lower, int32_t lower_length,
                                     const char* num, int32_t num_length,
                                     int32_t* out_len) {
  return mask_impl(context, data, data_len, upper, upper_length, lower, lower_length, num,
                   num_length, nullptr, 0, out_len);
}

GANDIVA_EXPORT
const char* mask_utf8_utf8_utf8(int64_t context, const char* in, int32_t length,
                                const char* upper, int32_t upper_len, const char* lower,
                                int32_t lower_len, int32_t* out_len) {
  return mask_impl(context, in, length, upper, upper_len, lower, lower_len, "n", 1,
                   nullptr, 0, out_len);
}

GANDIVA_EXPORT
const char* mask_utf8_utf8(int64_t context, const char* in, int32_t length,
                           const char* upper, int32_t upper_len, int32_t* out_len) {
  return mask_impl(context, in, length, upper, upper_len, "x", 1, "n", 1, nullptr, 0,
                   out_len);
}

GANDIVA_EXPORT
const char* mask_utf8(int64_t context, const char* in, int32_t length, int32_t* out_len) {
  return mask_impl(context, in, length, "X", 1, "x", 1, "n", 1, nullptr, 0, out_len);
}

// ---------------------------------------------------------------------------
// mask_internal
//
//   mask_internal(text, mode, char_count, upper, lower, digit, other) -> utf8
//
// A single entry point covering all five Hive masking modes with caller-supplied
// replacements, so an engine that normalises MASK / MASK_FIRST_N / MASK_LAST_N /
// MASK_SHOW_FIRST_N / MASK_SHOW_LAST_N into one call can evaluate every shape
// natively instead of one function per mode and arity.
//
// Classification matches the mask() family above: only uppercase letters (Lu),
// lowercase letters (Ll) and decimal digits (Nd) are masked. Everything else takes
// the `other` replacement, whose default leaves the character unchanged.
//
// Known divergence from a UTF-16 host implementation: character counts here are in
// Unicode codepoints, so a non-BMP character counts once rather than twice. This is
// visible only when a char_count boundary falls inside a surrogate pair.
// ---------------------------------------------------------------------------

enum MaskInternalSlot {
  kMaskSlotUpper = 0,
  kMaskSlotLower = 1,
  kMaskSlotDigit = 2,
  kMaskSlotOther = 3,
  kMaskSlotCount = 4
};

enum MaskInternalMode {
  kMaskModeFull = 0,
  kMaskModeFirstN,
  kMaskModeLastN,
  kMaskModeShowFirstN,
  kMaskModeShowLastN
};

// One resolved replacement. A zero length means "leave this class unchanged".
struct MaskInternalRepl {
  char bytes[4];
  int32_t len;
};

// "Leave unmasked" is spelled as an argument that parses to -1, i.e. '-' followed by
// any number of '0's and a final '1'. Anything else either parses to a different
// value or is not numeric at all, and is used as a replacement character.
static inline bool is_mask_internal_unmasked(const char* arg, int32_t len) {
  if (len < 2 || arg[0] != '-') {
    return false;
  }
  int32_t i = 1;
  while (i < len - 1 && arg[i] == '0') {
    i++;
  }
  return i == len - 1 && arg[i] == '1';
}

// Resolves one replacement argument: absent or empty takes the default, the unmasked
// spelling disables masking for that class, and anything else is truncated to its
// first character.
static inline bool mask_internal_make_repl(int64_t context, const char* arg,
                                           int32_t arg_len, int32_t default_cp,
                                           MaskInternalRepl* out) {
  int32_t codepoint = default_cp;
  if (arg != nullptr && arg_len > 0) {
    if (is_mask_internal_unmasked(arg, arg_len)) {
      codepoint = -1;
    } else {
      utf8proc_int32_t decoded = 0;
      auto char_len = utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(arg),
                                       arg_len, &decoded);
      if (char_len < 0) {
        gdv_fn_context_set_error_msg(context, utf8proc_errmsg(char_len));
        return false;
      }
      codepoint = decoded;
    }
  }

  if (codepoint < 0) {
    out->len = 0;
    return true;
  }
  auto encoded =
      utf8proc_encode_char(codepoint, reinterpret_cast<utf8proc_uint8_t*>(out->bytes));
  if (encoded <= 0) {
    gdv_fn_context_set_error_msg(context, "Invalid mask replacement character");
    return false;
  }
  out->len = static_cast<int32_t>(encoded);
  return true;
}

static inline int mask_internal_slot_of(utf8proc_int32_t codepoint) {
  switch (utf8proc_category(codepoint)) {
    case UTF8PROC_CATEGORY_LU:
      return kMaskSlotUpper;
    case UTF8PROC_CATEGORY_LL:
      return kMaskSlotLower;
    case UTF8PROC_CATEGORY_ND:
      return kMaskSlotDigit;
    default:
      return kMaskSlotOther;
  }
}

// Mode names are matched exactly and case-sensitively. All five have distinct
// lengths, so the dispatch is a switch plus one comparison.
static inline bool mask_internal_parse_mode(const char* mode, int32_t mode_len,
                                            int* out_mode) {
  switch (mode_len) {
    case 4:
      if (memcmp(mode, "FULL", 4) == 0) {
        *out_mode = kMaskModeFull;
        return true;
      }
      break;
    case 6:
      if (memcmp(mode, "LAST_N", 6) == 0) {
        *out_mode = kMaskModeLastN;
        return true;
      }
      break;
    case 7:
      if (memcmp(mode, "FIRST_N", 7) == 0) {
        *out_mode = kMaskModeFirstN;
        return true;
      }
      break;
    case 11:
      if (memcmp(mode, "SHOW_LAST_N", 11) == 0) {
        *out_mode = kMaskModeShowLastN;
        return true;
      }
      break;
    case 12:
      if (memcmp(mode, "SHOW_FIRST_N", 12) == 0) {
        *out_mode = kMaskModeShowFirstN;
        return true;
      }
      break;
    default:
      break;
  }
  return false;
}

// Counts codepoints and validates the encoding in one pass. Note the remaining length
// is passed to utf8proc_iterate: passing the full data_len, as the older mask stubs
// do, lets it read past the end of the buffer for a truncated trailing sequence.
static inline bool mask_internal_count_chars(int64_t context, const char* data,
                                             int32_t data_len, int32_t* out_count) {
  int32_t count = 0;
  int32_t pos = 0;
  utf8proc_int32_t codepoint = 0;
  while (pos < data_len) {
    auto char_len =
        utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(data + pos),
                         data_len - pos, &codepoint);
    if (char_len < 0) {
      gdv_fn_context_set_error_msg(context, utf8proc_errmsg(char_len));
      return false;
    }
    pos += static_cast<int32_t>(char_len);
    count++;
  }
  *out_count = count;
  return true;
}

GANDIVA_EXPORT
const char* gdv_fn_mask_internal(int64_t context, const char* data, int32_t data_len,
                                 const char* mode, int32_t mode_len, int32_t char_count,
                                 const char* upper, int32_t upper_len, const char* lower,
                                 int32_t lower_len, const char* digit, int32_t digit_len,
                                 const char* other, int32_t other_len, int32_t* out_len) {
  int mask_mode = kMaskModeFull;
  if (!mask_internal_parse_mode(mode, mode_len, &mask_mode)) {
    std::string err = "Unknown mask mode: " +
                      std::string(mode == nullptr ? "" : mode, std::max(mode_len, 0));
    gdv_fn_context_set_error_msg(context, err.c_str());
    *out_len = 0;
    return nullptr;
  }

  if (data_len <= 0) {
    *out_len = 0;
    return nullptr;
  }

  MaskInternalRepl repls[kMaskSlotCount];
  if (!mask_internal_make_repl(context, upper, upper_len, 'X', &repls[kMaskSlotUpper]) ||
      !mask_internal_make_repl(context, lower, lower_len, 'x', &repls[kMaskSlotLower]) ||
      !mask_internal_make_repl(context, digit, digit_len, 'n', &repls[kMaskSlotDigit]) ||
      !mask_internal_make_repl(context, other, other_len, -1, &repls[kMaskSlotOther])) {
    *out_len = 0;
    return nullptr;
  }

  int32_t max_repl_len = 1;
  bool any_masked = false;
  for (int slot = 0; slot < kMaskSlotCount; slot++) {
    max_repl_len = std::max(max_repl_len, repls[slot].len);
    any_masked = any_masked || repls[slot].len > 0;
  }

  // Every class is unmasked, so the result is the input verbatim.
  if (!any_masked) {
    *out_len = data_len;
    return data;
  }

  int32_t num_chars = 0;
  if (!mask_internal_count_chars(context, data, data_len, &num_chars)) {
    *out_len = 0;
    return nullptr;
  }

  // A negative char_count clamps to zero; FULL ignores it entirely.
  const int32_t count = std::max(char_count, 0);
  int32_t mask_begin = 0;
  int32_t mask_end = 0;
  switch (mask_mode) {
    case kMaskModeFull:
      mask_end = num_chars;
      break;
    case kMaskModeFirstN:
      mask_end = std::min(num_chars, count);
      break;
    case kMaskModeLastN:
      mask_begin = (num_chars <= count) ? 0 : num_chars - count;
      mask_end = num_chars;
      break;
    case kMaskModeShowFirstN:
      mask_begin = std::min(num_chars, count);
      mask_end = num_chars;
      break;
    case kMaskModeShowLastN:
      mask_end = (num_chars <= count) ? 0 : num_chars - count;
      break;
    default:
      break;
  }

  if (mask_begin >= mask_end) {
    *out_len = data_len;
    return data;
  }

  // A masked character contributes at most max_repl_len bytes and an unmasked one its
  // own width, and every character is at least one byte, so
  //   out <= sum(max(char_len_i, max_repl_len)) <= data_len + (max_repl_len - 1) * chars
  // which is exactly data_len for the common single-byte replacements.
  const int64_t alloc = static_cast<int64_t>(data_len) +
                        static_cast<int64_t>(max_repl_len - 1) *
                            static_cast<int64_t>(num_chars);
  if (alloc > std::numeric_limits<int32_t>::max()) {
    gdv_fn_context_set_error_msg(context, "Mask output would exceed the maximum size");
    *out_len = 0;
    return nullptr;
  }

  char* out = reinterpret_cast<char*>(
      gdv_fn_context_arena_malloc(context, static_cast<int32_t>(alloc)));
  if (out == nullptr) {
    gdv_fn_context_set_error_msg(context, "Could not allocate memory for output string");
    *out_len = 0;
    return nullptr;
  }

  int32_t bytes_read = 0;
  int32_t out_idx = 0;
  int32_t char_idx = 0;
  utf8proc_int32_t codepoint = 0;
  while (bytes_read < data_len) {
    // Already validated by mask_internal_count_chars, so char_len is positive.
    auto char_len =
        utf8proc_iterate(reinterpret_cast<const utf8proc_uint8_t*>(data + bytes_read),
                         data_len - bytes_read, &codepoint);
    const MaskInternalRepl* repl = nullptr;
    if (char_idx >= mask_begin && char_idx < mask_end) {
      repl = &repls[mask_internal_slot_of(codepoint)];
    }
    if (repl != nullptr && repl->len > 0) {
      memcpy(out + out_idx, repl->bytes, repl->len);
      out_idx += repl->len;
    } else {
      memcpy(out + out_idx, data + bytes_read, char_len);
      out_idx += static_cast<int32_t>(char_len);
    }
    bytes_read += static_cast<int32_t>(char_len);
    char_idx++;
  }

  *out_len = out_idx;
  return out;
}

int64_t gdv_fn_to_date_utf8_utf8(int64_t context_ptr, int64_t holder_ptr,
                                 const char* data, int data_len, bool in1_validity,
                                 const char* pattern, int pattern_len, bool in2_validity,
                                 bool* out_valid) {
  gandiva::ExecutionContext* context =
      reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);
  gandiva::ToDateHolder* holder = reinterpret_cast<gandiva::ToDateHolder*>(holder_ptr);
  return (*holder)(context, data, data_len, in1_validity, out_valid);
}

int64_t gdv_fn_to_date_utf8_utf8_int32(int64_t context_ptr, int64_t holder_ptr,
                                       const char* data, int data_len, bool in1_validity,
                                       const char* pattern, int pattern_len,
                                       bool in2_validity, int32_t suppress_errors,
                                       bool in3_validity, bool* out_valid) {
  gandiva::ExecutionContext* context =
      reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);
  gandiva::ToDateHolder* holder = reinterpret_cast<gandiva::ToDateHolder*>(holder_ptr);
  return (*holder)(context, data, data_len, in1_validity, out_valid);
}

int64_t gdv_fn_cast_intervalday_utf8(int64_t context_ptr, int64_t holder_ptr,
                                     const char* data, int data_len, bool in1_validity,
                                     bool* out_valid) {
  auto* context = reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);
  auto* holder = reinterpret_cast<gandiva::IntervalDaysHolder*>(holder_ptr);
  return (*holder)(context, data, data_len, in1_validity, out_valid);
}

int64_t gdv_fn_cast_intervalday_utf8_int32(int64_t context_ptr, int64_t holder_ptr,
                                           const char* data, int data_len,
                                           bool in1_validity, int32_t /*suppress_errors*/,
                                           bool /*in3_validity*/, bool* out_valid) {
  auto* context = reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);
  auto* holder = reinterpret_cast<gandiva::IntervalDaysHolder*>(holder_ptr);
  return (*holder)(context, data, data_len, in1_validity, out_valid);
}

int32_t gdv_fn_cast_intervalyear_utf8(int64_t context_ptr, int64_t holder_ptr,
                                      const char* data, int data_len, bool in1_validity,
                                      bool* out_valid) {
  auto* context = reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);
  auto* holder = reinterpret_cast<gandiva::IntervalYearsHolder*>(holder_ptr);
  return (*holder)(context, data, data_len, in1_validity, out_valid);
}

int32_t gdv_fn_cast_intervalyear_utf8_int32(int64_t context_ptr, int64_t holder_ptr,
                                            const char* data, int data_len,
                                            bool in1_validity,
                                            int32_t /*suppress_errors*/,
                                            bool /*in3_validity*/, bool* out_valid) {
  auto* context = reinterpret_cast<gandiva::ExecutionContext*>(context_ptr);
  auto* holder = reinterpret_cast<gandiva::IntervalYearsHolder*>(holder_ptr);
  return (*holder)(context, data, data_len, in1_validity, out_valid);
}

GANDIVA_EXPORT
gdv_timestamp to_utc_timezone_timestamp(int64_t context, gdv_timestamp time_milliseconds,
                                        const char* timezone, gdv_int32 length) {
  using arrow_vendored::date::locate_zone;
  using arrow_vendored::date::sys_time;
  using std::chrono::milliseconds;

  sys_time<milliseconds> tp{milliseconds{time_milliseconds}};
  try {
    const auto local_tz = locate_zone(std::string(timezone, length));
    gdv_timestamp offset = local_tz->get_info(tp).offset.count() * 1000;
    return time_milliseconds - static_cast<gdv_timestamp>(offset);
  } catch (...) {
    std::string e_msg = std::string(timezone, length) + " is an invalid time zone name.";
    gdv_fn_context_set_error_msg(context, e_msg.c_str());
    return 0;
  }
}

GANDIVA_EXPORT
gdv_timestamp from_utc_timezone_timestamp(gdv_int64 context,
                                          gdv_timestamp time_milliseconds,
                                          const char* timezone, gdv_int32 length) {
  using arrow_vendored::date::sys_time;
  using arrow_vendored::date::zoned_time;
  using std::chrono::milliseconds;

  const sys_time<milliseconds> tp{milliseconds{time_milliseconds}};
  try {
    const zoned_time<milliseconds> local_tz{std::string(timezone, length), tp};
    gdv_timestamp offset = local_tz.get_time_zone()->get_info(tp).offset.count() * 1000;
    return time_milliseconds + static_cast<gdv_timestamp>(offset);
  } catch (...) {
    std::string e_msg = std::string(timezone, length) + " is an invalid time zone name.";
    gdv_fn_context_set_error_msg(context, e_msg.c_str());
    return 0;
  }
}

GANDIVA_EXPORT
const char* gdv_mask_show_first_n_utf8_int32(int64_t context, const char* data,
                                             int32_t data_len, int32_t n_to_show,
                                             int32_t* out_len) {
  utf8proc_int32_t utf8_char_buffer;
  int num_of_chars = static_cast<int>(
      utf8proc_decompose(reinterpret_cast<const utf8proc_uint8_t*>(data), data_len,
                         &utf8_char_buffer, 1, UTF8PROC_STABLE));

  if (num_of_chars < 0) {
    gdv_fn_context_set_error_msg(context, utf8proc_errmsg(num_of_chars));
    *out_len = 0;
    return nullptr;
  }

  int32_t n_to_mask = num_of_chars - n_to_show;
  return gdv_mask_last_n_utf8_int32(context, data, data_len, n_to_mask, out_len);
}

GANDIVA_EXPORT
const char* gdv_mask_show_last_n_utf8_int32(int64_t context, const char* data,
                                            int32_t data_len, int32_t n_to_show,
                                            int32_t* out_len) {
  utf8proc_int32_t utf8_char_buffer;
  int num_of_chars = static_cast<int>(
      utf8proc_decompose(reinterpret_cast<const utf8proc_uint8_t*>(data), data_len,
                         &utf8_char_buffer, 1, UTF8PROC_STABLE));

  if (num_of_chars < 0) {
    gdv_fn_context_set_error_msg(context, utf8proc_errmsg(num_of_chars));
    *out_len = 0;
    return nullptr;
  }

  int32_t n_to_mask = num_of_chars - n_to_show;
  return gdv_mask_first_n_utf8_int32(context, data, data_len, n_to_mask, out_len);
}

ARROW_UNSUPPRESS_MISSING_DECLARATIONS_WARNING
}

// Close gandiva namespace before extern "C" functions, then reopen after
// These functions need to be at global scope for extern "C" linkage to work properly

// Legacy wrapper for string-based signatures (UTF8, UTF8) -> UTF8
// This is called by the LLVM engine with string calling convention
// WARNING: This function is for backward compatibility only. Encrypted binary
// data is not guaranteed to be valid UTF-8. Use binary signatures for new code.
extern "C" GANDIVA_EXPORT
const char* gdv_fn_aes_encrypt_ecb_legacy(int64_t context, const char* data,
                                          int32_t data_len,
                                          const char* key_data,
                                          int32_t key_data_len,
                                          int32_t* out_len) {
  // Delegate to the core implementation with ECB mode
  // This function is ECB-only, so we enforce the mode
  const char* mode = "AES-ECB";
  int32_t mode_len = 7;
  const char* result = gdv_fn_encrypt_dispatcher_3args(
      context, data, data_len, key_data, key_data_len, mode, mode_len, out_len);

  // Add null terminator for string compatibility
  // Note: This may not be valid UTF-8, but it's needed for string handling
  if (result != nullptr) {
    char* mutable_result = const_cast<char*>(result);
    mutable_result[*out_len] = '\0';
  }

  return result;
}

// Legacy wrapper for string-based signatures (UTF8, UTF8) -> UTF8
// This is called by the LLVM engine with string calling convention
// WARNING: This function is for backward compatibility only. Decrypted data
// may not be valid UTF-8. Use binary signatures for new code.
extern "C" GANDIVA_EXPORT
const char* gdv_fn_aes_decrypt_ecb_legacy(int64_t context, const char* data,
                                          int32_t data_len,
                                          const char* key_data,
                                          int32_t key_data_len,
                                          int32_t* out_len) {
  // Delegate to the core implementation with ECB mode
  // This function is ECB-only, so we enforce the mode
  const char* mode = "AES-ECB";
  int32_t mode_len = 7;
  const char* result = gdv_fn_decrypt_dispatcher_3args(
      context, data, data_len, key_data, key_data_len, mode, mode_len, out_len);

  // Add null terminator for string compatibility
  // Note: This may not be valid UTF-8, but it's needed for string handling
  if (result != nullptr) {
    char* mutable_result = const_cast<char*>(result);
    mutable_result[*out_len] = '\0';
  }

  return result;
}

// The 3- and 4-arg signatures exist to support optional IV and other arguments
extern "C" GANDIVA_EXPORT
const char* gdv_fn_encrypt_dispatcher_3args(
    int64_t context, const char* data, int32_t data_len, const char* key_data,
    int32_t key_data_len, const char* mode, int32_t mode_len,
    int32_t* out_len) {
  return gdv_fn_encrypt_dispatcher_5args(
      context, data, data_len, key_data, key_data_len, mode, mode_len, nullptr,
      0, nullptr, 0, out_len);
}

extern "C" GANDIVA_EXPORT
const char* gdv_fn_decrypt_dispatcher_3args(
    int64_t context, const char* data, int32_t data_len, const char* key_data,
    int32_t key_data_len, const char* mode, int32_t mode_len,
    int32_t* out_len) {
  return gdv_fn_decrypt_dispatcher_5args(
      context, data, data_len, key_data, key_data_len, mode, mode_len, nullptr,
      0, nullptr, 0, out_len);
}

extern "C" GANDIVA_EXPORT
const char* gdv_fn_encrypt_dispatcher_4args(
    int64_t context, const char* data, int32_t data_len, const char* key_data,
    int32_t key_data_len, const char* mode, int32_t mode_len,
    const char* iv_data, int32_t iv_data_len, int32_t* out_len) {
  return gdv_fn_encrypt_dispatcher_5args(
      context, data, data_len, key_data, key_data_len, mode, mode_len, iv_data,
      iv_data_len, nullptr, 0, out_len);
}

extern "C" GANDIVA_EXPORT
const char* gdv_fn_decrypt_dispatcher_4args(
    int64_t context, const char* data, int32_t data_len, const char* key_data,
    int32_t key_data_len, const char* mode, int32_t mode_len,
    const char* iv_data, int32_t iv_data_len, int32_t* out_len) {
  return gdv_fn_decrypt_dispatcher_5args(
      context, data, data_len, key_data, key_data_len, mode, mode_len, iv_data,
      iv_data_len, nullptr, 0, out_len);
}

extern "C" GANDIVA_EXPORT
const char* gdv_fn_encrypt_dispatcher_5args(
    int64_t context, const char* data, int32_t data_len, const char* key_data,
    int32_t key_data_len, const char* mode, int32_t mode_len,
    const char* iv_data, int32_t iv_data_len, const char* fifth_argument,
    int32_t fifth_argument_len, int32_t* out_len) {
  try {
    // Allocate extra 16 bytes for AES block padding (PKCS7 padding can add
    // up to 16 bytes for a 128-bit block cipher)
    // In cases of no-padding modes, this extra space is not used
    auto* output = reinterpret_cast<unsigned char*>(
        gdv_fn_context_arena_malloc(context, data_len + 16));
    if (output == nullptr) {
      throw std::runtime_error(
          "Memory allocation failed for encryption output");
    }

    int32_t cipher_len = gandiva::EncryptModeDispatcher::encrypt(
        data, data_len, key_data, key_data_len, mode, mode_len, iv_data,
        iv_data_len, fifth_argument, fifth_argument_len, output);

    *out_len = cipher_len;
    return reinterpret_cast<const char*>(output);
  } catch (const std::runtime_error& e) {
    gdv_fn_context_set_error_msg(context, e.what());
    *out_len = 0;
    return nullptr;
  }
}

extern "C" GANDIVA_EXPORT
const char* gdv_fn_decrypt_dispatcher_5args(
    int64_t context, const char* data, int32_t data_len, const char* key_data,
    int32_t key_data_len, const char* mode, int32_t mode_len,
    const char* iv_data, int32_t iv_data_len, const char* fifth_argument,
    int32_t fifth_argument_len, int32_t* out_len) {
  try {
    auto* output = reinterpret_cast<unsigned char*>(
        gdv_fn_context_arena_malloc(context, data_len));
    if (output == nullptr) {
      throw std::runtime_error(
          "Memory allocation failed for decryption output");
    }

    int32_t plaintext_len = gandiva::EncryptModeDispatcher::decrypt(
        data, data_len, key_data, key_data_len, mode, mode_len, iv_data,
        iv_data_len, fifth_argument, fifth_argument_len, output);

    *out_len = plaintext_len;
    return reinterpret_cast<const char*>(output);
  } catch (const std::runtime_error& e) {
    gdv_fn_context_set_error_msg(context, e.what());
    *out_len = 0;
    return nullptr;
  }
}

namespace gandiva {

arrow::Status ExportedStubFunctions::AddMappings(Engine* engine) const {
  std::vector<llvm::Type*> args;
  auto types = engine->types();

  // gdv_fn_random
  args = {types->i64_type()};
  engine->AddGlobalMappingForFunc("gdv_fn_random", types->double_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_random));

  args = {types->i64_type(), types->i32_type(), types->i1_type()};
  engine->AddGlobalMappingForFunc("gdv_fn_random_with_seed", types->double_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_random_with_seed));

  // gdv_fn_rand_integer
  args = {types->i64_type()};
  engine->AddGlobalMappingForFunc("gdv_fn_rand_integer", types->i32_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_rand_integer));

  args = {types->i64_type(), types->i32_type(), types->i1_type()};
  engine->AddGlobalMappingForFunc(
      "gdv_fn_rand_integer_with_range", types->i32_type(), args,
      reinterpret_cast<void*>(gdv_fn_rand_integer_with_range));

  args = {types->i64_type(), types->i32_type(), types->i1_type(), types->i32_type(),
          types->i1_type()};
  engine->AddGlobalMappingForFunc(
      "gdv_fn_rand_integer_with_min_max", types->i32_type(), args,
      reinterpret_cast<void*>(gdv_fn_rand_integer_with_min_max));

  // gdv_fn_dec_from_string
  args = {
      types->i64_type(),      // context
      types->i8_ptr_type(),   // const char* in
      types->i32_type(),      // int32_t in_length
      types->i32_ptr_type(),  // int32_t* precision_from_str
      types->i32_ptr_type(),  // int32_t* scale_from_str
      types->i64_ptr_type(),  // int64_t* dec_high_from_str
      types->i64_ptr_type(),  // int64_t* dec_low_from_str
  };

  engine->AddGlobalMappingForFunc("gdv_fn_dec_from_string",
                                  types->i32_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_dec_from_string));

  // gdv_fn_dec_to_string
  args = {
      types->i64_type(),      // context
      types->i64_type(),      // int64_t x_high
      types->i64_type(),      // int64_t x_low
      types->i32_type(),      // int32_t x_scale
      types->i64_ptr_type(),  // int64_t* dec_str_len
  };

  engine->AddGlobalMappingForFunc("gdv_fn_dec_to_string",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_dec_to_string));

  // gdv_fn_in_expr_lookup_int32
  args = {types->i64_type(),  // int64_t in holder ptr
          types->i32_type(),  // int32 value
          types->i1_type()};  // bool in_validity

  engine->AddGlobalMappingForFunc("gdv_fn_in_expr_lookup_int32",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_in_expr_lookup_int32));

  // gdv_fn_in_expr_lookup_int64
  args = {types->i64_type(),  // int64_t in holder ptr
          types->i64_type(),  // int64 value
          types->i1_type()};  // bool in_validity

  engine->AddGlobalMappingForFunc("gdv_fn_in_expr_lookup_int64",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_in_expr_lookup_int64));

  // gdv_fn_in_expr_lookup_decimal
  args = {types->i64_type(),  // int64_t in holder ptr
          types->i64_type(),  // high decimal value
          types->i64_type(),  // low decimal value
          types->i32_type(),  // decimal precision value
          types->i32_type(),  // decimal scale value
          types->i1_type()};  // bool in_validity

  engine->AddGlobalMappingForFunc("gdv_fn_in_expr_lookup_decimal",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_in_expr_lookup_decimal));

  // gdv_fn_in_expr_lookup_utf8
  args = {types->i64_type(),     // int64_t in holder ptr
          types->i8_ptr_type(),  // const char* value
          types->i32_type(),     // int value_len
          types->i1_type()};     // bool in_validity

  engine->AddGlobalMappingForFunc("gdv_fn_in_expr_lookup_utf8",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_in_expr_lookup_utf8));
  // gdv_fn_in_expr_lookup_float
  args = {types->i64_type(),    // int64_t in holder ptr
          types->float_type(),  // float value
          types->i1_type()};    // bool in_validity

  engine->AddGlobalMappingForFunc("gdv_fn_in_expr_lookup_float",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_in_expr_lookup_float));
  // gdv_fn_in_expr_lookup_double
  args = {types->i64_type(),     // int64_t in holder ptr
          types->double_type(),  // double value
          types->i1_type()};     // bool in_validity

  engine->AddGlobalMappingForFunc("gdv_fn_in_expr_lookup_double",
                                  types->i1_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_in_expr_lookup_double));
  // gdv_fn_populate_varlen_vector
  args = {types->i64_type(),      // int64_t execution_context
          types->i8_ptr_type(),   // int8_t* data ptr
          types->i32_ptr_type(),  // int32_t* offsets ptr
          types->i64_type(),      // int64_t slot
          types->i8_ptr_type(),   // const char* entry_buf
          types->i32_type()};     // int32_t entry__len

  engine->AddGlobalMappingForFunc("gdv_fn_populate_varlen_vector",
                                  types->i32_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_populate_varlen_vector));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castINT_utf8", types->i32_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_castINT_utf8));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castBIGINT_utf8", types->i64_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_castBIGINT_utf8));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castFLOAT4_utf8", types->float_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_castFLOAT4_utf8));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castFLOAT8_utf8", types->double_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_castFLOAT8_utf8));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castINT_varbinary", types->i32_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_castINT_varbinary));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castBIGINT_varbinary", types->i64_type(), args,
                                  reinterpret_cast<void*>(gdv_fn_castBIGINT_varbinary));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castFLOAT4_varbinary", types->float_type(),
                                  args,
                                  reinterpret_cast<void*>(gdv_fn_castFLOAT4_varbinary));

  args = {types->i64_type(),     // int64_t context_ptr
          types->i8_ptr_type(),  // const char* data
          types->i32_type()};    // int32_t lenr

  engine->AddGlobalMappingForFunc("gdv_fn_castFLOAT8_varbinary", types->double_type(),
                                  args,
                                  reinterpret_cast<void*>(gdv_fn_castFLOAT8_varbinary));

  // gdv_fn_base64_encode_utf8
  args = {
      types->i64_type(),      // context
      types->i8_ptr_type(),   // in
      types->i32_type(),      // in_len
      types->i32_ptr_type(),  // out_len
  };

  engine->AddGlobalMappingForFunc("gdv_fn_base64_encode_binary",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_base64_encode_binary));

  // gdv_fn_base64_decode_utf8
  args = {
      types->i64_type(),      // context
      types->i8_ptr_type(),   // in
      types->i32_type(),      // in_len
      types->i32_ptr_type(),  // out_len
  };

  engine->AddGlobalMappingForFunc("gdv_fn_base64_decode_utf8",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_base64_decode_utf8));

  // gdv_fn_aes_encrypt_ecb_legacy (wrapper for string-based signatures)
  // Note: Mode is hardcoded internally as "ECB", not passed as parameter
  // Function signature: (context, data, data_len, key_data, key_data_len, out_len)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("gdv_fn_aes_encrypt_ecb_legacy",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_aes_encrypt_ecb_legacy));

  // gdv_fn_aes_decrypt_ecb_legacy (wrapper for string-based signatures)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("gdv_fn_aes_decrypt_ecb_legacy",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_aes_decrypt_ecb_legacy));

  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i8_ptr_type(),  // iv (binary string)
      types->i32_type(),     // iv_length
      types->i8_ptr_type(),  // fifth_argument (binary string)
      types->i32_type(),     // fifth_argument_length
      types->i32_ptr_type()  // out_length
  };

  // gdv_fn_encrypt_dispatcher_3args (data, key, mode)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_encrypt_dispatcher_3args",
      types->i8_ptr_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_encrypt_dispatcher_3args));

  // gdv_fn_decrypt_dispatcher_3args (data, key, mode)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_decrypt_dispatcher_3args",
      types->i8_ptr_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_decrypt_dispatcher_3args));

  // gdv_fn_encrypt_dispatcher_4args (data, key, mode, iv)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i8_ptr_type(),  // iv (binary string)
      types->i32_type(),     // iv_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_encrypt_dispatcher_4args",
      types->i8_ptr_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_encrypt_dispatcher_4args));

  // gdv_fn_decrypt_dispatcher_4args (data, key, mode, iv)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i8_ptr_type(),  // iv (binary string)
      types->i32_type(),     // iv_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_decrypt_dispatcher_4args",
      types->i8_ptr_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_decrypt_dispatcher_4args));

  // gdv_fn_encrypt_dispatcher_5args (data, key, mode, iv,
  // fifth_argument)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i8_ptr_type(),  // iv (binary string)
      types->i32_type(),     // iv_length
      types->i8_ptr_type(),  // fifth_argument (binary string)
      types->i32_type(),     // fifth_argument_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_encrypt_dispatcher_5args",
      types->i8_ptr_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_encrypt_dispatcher_5args));

  // gdv_fn_decrypt_dispatcher_5args (data, key, mode, iv,
  // fifth_argument)
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i8_ptr_type(),  // key_data
      types->i32_type(),     // key_data_length
      types->i8_ptr_type(),  // mode (binary string)
      types->i32_type(),     // mode_length
      types->i8_ptr_type(),  // iv (binary string)
      types->i32_type(),     // iv_length
      types->i8_ptr_type(),  // fifth_argument (binary string)
      types->i32_type(),     // fifth_argument_length
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_decrypt_dispatcher_5args",
      types->i8_ptr_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_decrypt_dispatcher_5args));

  // gdv_mask_first_n and gdv_mask_last_n
  std::vector<llvm::Type*> mask_args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i32_type(),     // n_to_mask
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("gdv_mask_first_n_utf8_int32",
                                  types->i8_ptr_type() /*return_type*/, mask_args,
                                  reinterpret_cast<void*>(gdv_mask_first_n_utf8_int32));

  engine->AddGlobalMappingForFunc("gdv_mask_last_n_utf8_int32",
                                  types->i8_ptr_type() /*return_type*/, mask_args,
                                  reinterpret_cast<void*>(gdv_mask_last_n_utf8_int32));

  // gdv_fn_crc_32_utf8
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // const char*
      types->i32_type()      // value_length
  };

  engine->AddGlobalMappingForFunc("gdv_fn_crc_32_utf8", types->i64_type() /*return_type*/,
                                  args, reinterpret_cast<void*>(gdv_fn_crc_32_utf8));

  // gdv_fn_crc_32_binary
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // const char*
      types->i32_type()      // value_length
  };

  engine->AddGlobalMappingForFunc("gdv_fn_crc_32_binary",
                                  types->i64_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_crc_32_binary));

  // gdv_fn_to_date_utf8_utf8
  args = {types->i64_type(),                   // int64_t execution_context
          types->i64_type(),                   // int64_t holder_ptr
          types->i8_ptr_type(),                // const char* data
          types->i32_type(),                   // int data_len
          types->i1_type(),                    // bool in1_validity
          types->i8_ptr_type(),                // const char* pattern
          types->i32_type(),                   // int pattern_len
          types->i1_type(),                    // bool in2_validity
          types->ptr_type(types->i8_type())};  // bool* out_valid

  engine->AddGlobalMappingForFunc("gdv_fn_to_date_utf8_utf8",
                                  types->i64_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_to_date_utf8_utf8));

  // gdv_fn_to_date_utf8_utf8_int32
  args = {types->i64_type(),                   // int64_t execution_context
          types->i64_type(),                   // int64_t holder_ptr
          types->i8_ptr_type(),                // const char* data
          types->i32_type(),                   // int data_len
          types->i1_type(),                    // bool in1_validity
          types->i8_ptr_type(),                // const char* pattern
          types->i32_type(),                   // int pattern_len
          types->i1_type(),                    // bool in2_validity
          types->i32_type(),                   // int32_t suppress_errors
          types->i1_type(),                    // bool in3_validity
          types->ptr_type(types->i8_type())};  // bool* out_valid

  engine->AddGlobalMappingForFunc(
      "gdv_fn_to_date_utf8_utf8_int32", types->i64_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_to_date_utf8_utf8_int32));

  // gdv_fn_cast_intervalday_utf8
  args = {
      types->i64_type(),                 // context
      types->i64_type(),                 // holder
      types->i8_ptr_type(),              // data
      types->i32_type(),                 // data_len
      types->i1_type(),                  // data validity
      types->ptr_type(types->i8_type())  // out validity
  };

  engine->AddGlobalMappingForFunc("gdv_fn_cast_intervalday_utf8",
                                  types->i64_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_cast_intervalday_utf8));

  // gdv_fn_cast_intervalday_utf8_int32
  args = {
      types->i64_type(),                 // context
      types->i64_type(),                 // holder
      types->i8_ptr_type(),              // data
      types->i32_type(),                 // data_len
      types->i1_type(),                  // data validity
      types->i32_type(),                 // suppress_error
      types->i1_type(),                  // suppress_error validity
      types->ptr_type(types->i8_type())  // out validity
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_cast_intervalday_utf8_int32", types->i64_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_cast_intervalday_utf8_int32));

  // gdv_fn_cast_intervalyear_utf8
  args = {
      types->i64_type(),                 // context
      types->i64_type(),                 // holder
      types->i8_ptr_type(),              // data
      types->i32_type(),                 // data_len
      types->i1_type(),                  // data validity
      types->ptr_type(types->i8_type())  // out validity
  };

  engine->AddGlobalMappingForFunc("gdv_fn_cast_intervalyear_utf8",
                                  types->i32_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_cast_intervalyear_utf8));

  // gdv_fn_cast_intervalyear_utf8_int32
  args = {
      types->i64_type(),                 // context
      types->i64_type(),                 // holder
      types->i8_ptr_type(),              // data
      types->i32_type(),                 // data_len
      types->i1_type(),                  // data validity
      types->i32_type(),                 // suppress_error
      types->i1_type(),                  // suppress_error validity
      types->ptr_type(types->i8_type())  // out validity
  };

  engine->AddGlobalMappingForFunc(
      "gdv_fn_cast_intervalyear_utf8_int32", types->i32_type() /*return_type*/, args,
      reinterpret_cast<void*>(gdv_fn_cast_intervalyear_utf8_int32));

  // to_utc_timezone_timestamp
  args = {
      types->i64_type(),     // context
      types->i64_type(),     // timestamp
      types->i8_ptr_type(),  // timezone
      types->i32_type()      // length
  };

  engine->AddGlobalMappingForFunc("to_utc_timezone_timestamp",
                                  types->i64_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(to_utc_timezone_timestamp));

  // from_utc_timezone_timestamp
  args = {
      types->i64_type(),     // context
      types->i64_type(),     // timestamp
      types->i8_ptr_type(),  // timezone
      types->i32_type()      // length
  };

  engine->AddGlobalMappingForFunc("from_utc_timezone_timestamp",
                                  types->i64_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(from_utc_timezone_timestamp));

  // mask-show-n
  mask_args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_length
      types->i32_type(),     // n_to_show
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc(
      "gdv_mask_show_first_n_utf8_int32", types->i8_ptr_type() /*return_type*/, mask_args,
      reinterpret_cast<void*>(gdv_mask_show_first_n_utf8_int32));

  engine->AddGlobalMappingForFunc(
      "gdv_mask_show_last_n_utf8_int32", types->i8_ptr_type() /*return_type*/, mask_args,
      reinterpret_cast<void*>(gdv_mask_show_last_n_utf8_int32));

  // gdv_fn_mask_internal
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_len
      types->i8_ptr_type(),  // mode
      types->i32_type(),     // mode_len
      types->i32_type(),     // char_count
      types->i8_ptr_type(),  // upper
      types->i32_type(),     // upper_len
      types->i8_ptr_type(),  // lower
      types->i32_type(),     // lower_len
      types->i8_ptr_type(),  // digit
      types->i32_type(),     // digit_len
      types->i8_ptr_type(),  // other
      types->i32_type(),     // other_len
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("gdv_fn_mask_internal",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(gdv_fn_mask_internal));

  // mask_utf8_utf8_utf8_utf8_utf8
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_len
      types->i8_ptr_type(),  // upper
      types->i32_type(),     // upper_len
      types->i8_ptr_type(),  // lower
      types->i32_type(),     // lower_len
      types->i8_ptr_type(),  // num
      types->i32_type(),     // num_len
      types->i8_ptr_type(),  // other
      types->i32_type(),     // other_len
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("mask_utf8_utf8_utf8_utf8_utf8",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(mask_utf8_utf8_utf8_utf8_utf8));

  // mask_utf8_utf8_utf8_utf8
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_len
      types->i8_ptr_type(),  // upper
      types->i32_type(),     // upper_len
      types->i8_ptr_type(),  // lower
      types->i32_type(),     // lower_len
      types->i8_ptr_type(),  // num
      types->i32_type(),     // num_len
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("mask_utf8_utf8_utf8_utf8",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(mask_utf8_utf8_utf8_utf8));

  // mask_utf8_utf8_utf8
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_len
      types->i8_ptr_type(),  // upper
      types->i32_type(),     // upper_len
      types->i8_ptr_type(),  // lower
      types->i32_type(),     // lower_len
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("mask_utf8_utf8_utf8",
                                  types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(mask_utf8_utf8_utf8));

  // mask_utf8_utf8
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_len
      types->i8_ptr_type(),  // upper
      types->i32_type(),     // upper_len
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("mask_utf8_utf8", types->i8_ptr_type() /*return_type*/,
                                  args, reinterpret_cast<void*>(mask_utf8_utf8));

  // mask_utf8
  args = {
      types->i64_type(),     // context
      types->i8_ptr_type(),  // data
      types->i32_type(),     // data_len
      types->i32_ptr_type()  // out_length
  };

  engine->AddGlobalMappingForFunc("mask_utf8", types->i8_ptr_type() /*return_type*/, args,
                                  reinterpret_cast<void*>(mask_utf8));
  return arrow::Status::OK();
}
}  // namespace gandiva
