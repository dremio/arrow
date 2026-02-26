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

#include <vector>

#include "arrow/memory_pool.h"
#include "arrow/status.h"
#include "gandiva/execution_context.h"
#include "gandiva/precompiled/types.h"
#include "gandiva/projector.h"
#include "gandiva/tests/test_util.h"
#include "gandiva/tree_expr_builder.h"

namespace gandiva {

using arrow::boolean;
using arrow::float32;
using arrow::float64;
using arrow::int32;
using arrow::int64;
using arrow::utf8;
using std::string;
using std::vector;

class TestList : public ::testing::Test {
 public:
  void SetUp() { pool_ = arrow::default_memory_pool(); }

 protected:
  arrow::MemoryPool* pool_;
};

template <class ValueType, class ArrayType>
void _build_list_array(const vector<ValueType>& values, const vector<int64_t>& length,
                       const vector<bool>& validity, arrow::MemoryPool* pool,
                       ArrayPtr* array, const vector<bool>& innerValidity = {}) {
  size_t sum = 0;
  for (auto& len : length) {
    sum += len;
  }
  EXPECT_TRUE(values.size() == sum);
  EXPECT_TRUE(length.size() == validity.size());

  auto value_builder = std::make_shared<ArrayType>(pool);
  auto builder = std::make_shared<arrow::ListBuilder>(pool, value_builder);
  int i = 0;
  for (size_t l = 0; l < length.size(); l++) {
    if (validity[l]) {
      auto status = builder->Append();
      for (int j = 0; j < length[l]; j++) {
        if (innerValidity.size() > (size_t)j && innerValidity[j] == false) {
          auto v = value_builder->AppendNull();
        } else {
          ASSERT_OK(value_builder->Append(values[i]));
        }
        i++;
      }
    } else {
      ASSERT_OK(builder->AppendNull());
      for (int j = 0; j < length[l]; j++) {
        i++;
      }
    }
  }
  ASSERT_OK(builder->Finish(array));
}

template <class ValueType, class ArrayType>
void _build_list_array2(const vector<ValueType>& values, const vector<int64_t>& length,
                        const vector<bool>& validity, const vector<bool>& innerValidity,
                        arrow::MemoryPool* pool, ArrayPtr* array) {
  return _build_list_array<ValueType, ArrayType>(values, length, validity, pool, array);
}

/*
 * expression:
 *      input: a
 *      output: res
 * typeof(a) can be list<binary_like> / list<int> / list<float>
 */
void _test_list_type_field_alias(DataTypePtr type, ArrayPtr array,
                                 arrow::MemoryPool* pool, int num_records = 5) {
  auto field_a = field("a", type);
  auto schema = arrow::schema({field_a});
  auto result = field("res", type);

  std::cout << array->ToString() << std::endl;
  assert(array->length() == num_records);

  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array});

  // Make expression
  std::cout << "Make expression" << std::endl;
  auto field_a_node = TreeExprBuilder::MakeField(field_a);
  auto expr = TreeExprBuilder::MakeExpression(field_a_node, result);

  std::cout << "Build a projector for the expressions." << std::endl;
  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  std::cout << "status message: " << status.message() << std::endl;
  EXPECT_TRUE(status.ok()) << status.message();

  std::cout << "Evaluate expression" << std::endl;
  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool, &outputs);
  EXPECT_TRUE(status.ok()) << status.message();

  std::cout << "Check results" << std::endl;
  EXPECT_ARROW_ARRAY_EQUALS(array, outputs[0]);
  // EXPECT_ARROW_ARRAY_EQUALS will not check the length of child data, but
  // ArrayData::Slice method will check length. ArrayData::ToString method will call
  // ArrayData::Slice method
  EXPECT_TRUE(array->ToString() == outputs[0]->ToString());
  EXPECT_TRUE(array->null_count() == outputs[0]->null_count());
}

/*
TEST_F(TestList, TestArrayRemove) {
  // schema for input fields
  auto field_b = field("b", int32());

  auto field_a = field("a", list(int32()));
  auto schema = arrow::schema({field_a, field_b});

  // output fields
  auto res = field("res", list(int32()));

  // Create a row-batch with some sample data
  int num_records = 2;
  auto array_b =
      MakeArrowArrayInt32({42, 42}, {true, true});

  ArrayPtr array_a;
  _build_list_array2<int32_t, arrow::Int32Builder>(
      {10, 42, 30, 42, 70, 80},
      {3, 3}, {true, true}, {true, true, true, true, true, true}, pool_, &array_a);

  // expected output
  ArrayPtr exp1;
  _build_list_array2<int32_t, arrow::Int32Builder>(
      {10, 30, 70, 80},
      {2, 2}, {true, true}, {true, true, true, true}, pool_, &exp1);


  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array_a, array_b});

  auto expr = TreeExprBuilder::MakeExpression("array_remove", {field_a, field_b}, res);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok()) << status.message();

  std::cout << "LR Test 2 " << std::endl;
  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok()) << status.message();
  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp1, outputs.at(0));

  //Try the second method.
  arrow::ArrayDataVector outputs2;
  std::shared_ptr<arrow::DataType> listDt = std::make_shared<arrow::Int32Type>();
  std::shared_ptr<arrow::DataType> dt = std::make_shared<arrow::ListType>(listDt);


       int num_records2 = 5;
        std::vector<std::shared_ptr<arrow::Buffer>> buffers;

  int64_t size = 20;
  auto bitmap_buffer =  arrow::AllocateBuffer(size, pool_);
  buffers.push_back(*std::move(bitmap_buffer));
    auto offsets_len = arrow::bit_util::BytesForBits((num_records2 + 1) * 32);

    auto offsets_buffer = arrow::AllocateBuffer(offsets_len*10, pool_);
    buffers.push_back(*std::move(offsets_buffer));

std::vector<std::shared_ptr<arrow::Buffer>> buffers2;
auto bitmap_buffer2 =  arrow::AllocateBuffer(size, pool_);
  buffers2.push_back(*std::move(bitmap_buffer2));

    auto offsets_buffer2 = arrow::AllocateBuffer(offsets_len, pool_);
    buffers2.push_back(*std::move(offsets_buffer2));
std::shared_ptr<arrow::DataType> dt2 = std::make_shared<arrow::Int32Type>();

        auto array_data_child = arrow::ArrayData::Make(dt2, num_records2, buffers2, 0, 0);
        array_data_child->buffers = std::move(buffers2);

        std::vector<std::shared_ptr<arrow::ArrayData>> kids;
        kids.push_back(array_data_child);


auto array_data = arrow::ArrayData::Make(dt, num_records2, buffers, kids, 0, 0);
array_data->buffers = std::move(buffers);
outputs2.push_back(array_data);


  status = projector->Evaluate(*(in_batch.get()), outputs2);
  EXPECT_TRUE(status.ok()) << status.message();
  arrow::ArrayData ad = *outputs2.at(0);
  arrow::ArraySpan sp(*ad.child_data.at(0));
  EXPECT_ARROW_ARRAY_EQUALS(exp1, sp.ToArray());




for (auto& array_data : outputs2) {
      auto child_data = array_data->child_data[0];
      int64_t child_data_size = 1;
      if (arrow::is_binary_like(child_data->type->id())) {
        child_data_size = child_data->buffers[1]->size() / 4 - 1;
      } else if (child_data->type->id() == arrow::Type::INT32) {
        child_data_size = child_data->buffers[1]->size() / 4;
      } else if (child_data->type->id() == arrow::Type::INT64) {
        child_data_size = child_data->buffers[1]->size() / 8;
      } else if (child_data->type->id() == arrow::Type::FLOAT) {
        child_data_size = child_data->buffers[1]->size() / 4;
      } else if (child_data->type->id() == arrow::Type::DOUBLE) {
        child_data_size = child_data->buffers[1]->size() / 8;
      }
      auto new_child_data = arrow::ArrayData::Make(
          child_data->type, child_data_size, child_data->buffers, child_data->offset);
      array_data = arrow::ArrayData::Make(array_data->type, array_data->length,
                                          array_data->buffers, {new_child_data},
                                          array_data->null_count, array_data->offset);


    auto newArray = arrow::MakeArray(array_data);
      //arrow::ArraySpan sp(newArray);
  EXPECT_ARROW_ARRAY_EQUALS(exp1, newArray);
}


{
  std::shared_ptr<arrow::DataType> listDt = std::make_shared<arrow::Int32Type>();
  std::shared_ptr<arrow::DataType> dt = std::make_shared<arrow::ListType>(listDt);

ArrayDataPtr output_data;
      auto s = projector->AllocArrayData(dt, num_records2, pool_, &output_data);
      ArrayDataVector output_data_vecs;
    output_data_vecs.push_back(output_data);

      status = projector->Evaluate(*(in_batch.get()), output_data_vecs);
  EXPECT_TRUE(status.ok()) << status.message();
  arrow::ArraySpan sp(*output_data_vecs.at(0));
  EXPECT_ARROW_ARRAY_EQUALS(exp1, sp.ToArray());
 }
}

TEST_F(TestList, TestListArrayInt32) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<gdv_int64>(&ctx);
  int32_t data[] = {11, 2, 23, 42};
  int32_t entry_offsets_len = 4;
  int32_t contains_data = 42;

  EXPECT_EQ(
      array_int32_contains_int32(ctx_ptr, data, entry_offsets_len,
                               contains_data),
      true);
}


TEST_F(TestList, TestListInt32LiteralContains) {
  // schema for input fields
  auto field_a = field("a", list(int32()));
  auto field_b = field("b", int32());
  auto schema = arrow::schema({field_a, field_b});

  // output fields
  auto res = field("res", boolean());

  // Create a row-batch with some sample data
  int num_records = 5;
  ArrayPtr array_a;
    _build_list_array<int32_t, arrow::Int32Builder>(
      {1, 5, 19, 42, 57},
      {1, 1, 1, 1, 1}, {true, true, true, true, true}, pool_, &array_a);

  auto array_b =
      MakeArrowArrayInt32({42, 42, 42, 42, 42});

  // expected output
  auto exp = MakeArrowArrayBool({false, false, false, true, false},
                                {true, true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array_a, array_b});

  std::vector<NodePtr> field_nodes;
  auto node = TreeExprBuilder::MakeField(field_a);
  field_nodes.push_back(node);

  auto node2 = TreeExprBuilder::MakeLiteral(42);
  field_nodes.push_back(node2);

  auto func_node = TreeExprBuilder::MakeFunction("array_contains", field_nodes,
res->type()); auto expr = TreeExprBuilder::MakeExpression(func_node, res);
  ////////

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok()) << status.message();

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok()) << status.message();

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestList, TestListInt32Contains) {
  // schema for input fields
  auto field_a = field("a", list(int32()));
  auto field_b = field("b", int32());
  auto schema = arrow::schema({field_a, field_b});

  // output fields
  auto res = field("res", boolean());

  // Create a row-batch with some sample data
  int num_records = 5;
  ArrayPtr array_a;
    _build_list_array<int32_t, arrow::Int32Builder>(
      {1, 5, 19, 42, 57},
      {1, 1, 1, 1, 1}, {true, true, true, true, true}, pool_, &array_a);

  auto array_b =
      MakeArrowArrayInt32({42, 42, 42, 42, 42});

  // expected output
  auto exp = MakeArrowArrayBool({false, false, false, true, false},
                                {true, true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array_a, array_b});

  // build expressions.
  // array_contains(a, b)
  auto expr = TreeExprBuilder::MakeExpression("array_contains", {field_a, field_b}, res);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok()) << status.message();

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok()) << status.message();

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestList, TestListFloat32) {
  ArrayPtr array;
  _build_list_array<float, arrow::FloatBuilder>(
      {1.1f, 11.1f, 22.2f, 111.1f, 222.2f, 333.3f, 1111.1f, 2222.2f, 3333.3f, 4444.4f,
       11111.1f, 22222.2f, 33333.3f, 44444.4f, 55555.5f},
      {1, 2, 3, 4, 5}, {true, true, true, true, true}, pool_, &array);
  _test_list_type_field_alias(list(float32()), array, pool_);
}

TEST_F(TestList, TestListFloat64) {
  ArrayPtr array;
  _build_list_array<double, arrow::DoubleBuilder>(
      {1.1, 1.11, 2.22, 1.111, 2.222, 3.333, 1.1111, 2.2222, 3.3333, 4.4444, 1.11111,
       2.22222, 3.33333, 4.44444, 5.55555},
      {1, 2, 4, 3, 5}, {true, false, true, true, true}, pool_, &array);
  _test_list_type_field_alias(list(float64()), array, pool_);
}*/

}  // namespace gandiva
