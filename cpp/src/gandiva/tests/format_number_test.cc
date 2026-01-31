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

#include "arrow/memory_pool.h"

#include "gandiva/projector.h"
#include "gandiva/tests/test_util.h"
#include "gandiva/tree_expr_builder.h"

namespace gandiva {

using arrow::float64;
using arrow::int32;
using arrow::utf8;

class TestFormatNumber : public ::testing::Test {
 public:
  void SetUp() override { pool_ = arrow::default_memory_pool(); }

 protected:
  arrow::MemoryPool* pool_;
};

TEST_F(TestFormatNumber, BasicLiteralDecimalPlaces) {
  auto field_value = field("value", float64());
  auto schema = arrow::schema({field_value});

  auto field_result = field("res", utf8());

  auto node_value = TreeExprBuilder::MakeField(field_value);
  auto node_decimal_places = TreeExprBuilder::MakeLiteral(static_cast<int32_t>(2));
  auto fn =
      TreeExprBuilder::MakeFunction("format_number", {node_value, node_decimal_places},
                                   utf8());
  auto expr = TreeExprBuilder::MakeExpression(fn, field_result);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok()) << status.message();

  const int num_records = 5;
  auto array_value =
      MakeArrowArrayFloat64({1234567.891, 12.0, -1234.5, 0.0, 42.1},
                            {true, true, true, true, false});
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array_value});

  auto exp = MakeArrowArrayUtf8({"1,234,567.89", "12.00", "-1,234.50", "0.00", ""},
                                {true, true, true, true, false});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok()) << status.message();

  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestFormatNumber, DecimalPlacesFromField) {
  auto field_value = field("value", float64());
  auto field_decimal_places = field("decimal_places", int32());
  auto schema = arrow::schema({field_value, field_decimal_places});

  auto field_result = field("res", utf8());

  auto node_value = TreeExprBuilder::MakeField(field_value);
  auto node_decimal_places = TreeExprBuilder::MakeField(field_decimal_places);
  auto fn =
      TreeExprBuilder::MakeFunction("format_number", {node_value, node_decimal_places},
                                   utf8());
  auto expr = TreeExprBuilder::MakeExpression(fn, field_result);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok()) << status.message();

  const int num_records = 4;
  auto array_value = MakeArrowArrayFloat64({1234.0, 1234.0, 1234.0, 1234.0},
                                           {true, true, true, true});
  auto array_decimal_places =
      MakeArrowArrayInt32({0, 1, 2, 3}, {true, true, true, true});
  auto in_batch =
      arrow::RecordBatch::Make(schema, num_records, {array_value, array_decimal_places});

  auto exp = MakeArrowArrayUtf8({"1,234", "1,234.0", "1,234.00", "1,234.000"},
                                {true, true, true, true});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok()) << status.message();

  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestFormatNumber, NegativeDecimalPlacesErrors) {
  auto field_value = field("value", float64());
  auto field_decimal_places = field("decimal_places", int32());
  auto schema = arrow::schema({field_value, field_decimal_places});

  auto field_result = field("res", utf8());

  auto node_value = TreeExprBuilder::MakeField(field_value);
  auto node_decimal_places = TreeExprBuilder::MakeField(field_decimal_places);
  auto fn =
      TreeExprBuilder::MakeFunction("format_number", {node_value, node_decimal_places},
                                   utf8());
  auto expr = TreeExprBuilder::MakeExpression(fn, field_result);

  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok()) << status.message();

  const int num_records = 1;
  auto array_value = MakeArrowArrayFloat64({1.0}, {true});
  auto array_decimal_places = MakeArrowArrayInt32({-1}, {true});
  auto in_batch =
      arrow::RecordBatch::Make(schema, num_records, {array_value, array_decimal_places});

  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_FALSE(status.ok()) << status.message();
  EXPECT_NE(status.message().find("Decimal places cannot be negative"),
            std::string::npos);
}

}  // namespace gandiva
