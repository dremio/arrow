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
#include "arrow/status.h"

#include "gandiva/projector.h"
#include "gandiva/tests/test_util.h"
#include "gandiva/tree_expr_builder.h"

namespace gandiva {

using arrow::boolean;
using arrow::float32;
using arrow::int32;

class TestIfExpr : public ::testing::Test {
 public:
  void SetUp() { pool_ = arrow::default_memory_pool(); }

 protected:
  arrow::MemoryPool* pool_;
};

TEST_F(TestIfExpr, TestSimple) {
  // schema for input fields
  auto fielda = field("a", int32());
  auto fieldb = field("b", int32());
  auto schema = arrow::schema({fielda, fieldb});

  // output fields
  auto field_result = field("res", int32());

  // build expression.
  // if (a > b)
  //   a
  // else
  //   b
  auto node_a = TreeExprBuilder::MakeField(fielda);
  auto node_b = TreeExprBuilder::MakeField(fieldb);
  auto condition =
      TreeExprBuilder::MakeFunction("greater_than", {node_a, node_b}, boolean());
  auto if_node = TreeExprBuilder::MakeIf(condition, node_a, node_b, int32());

  auto expr = TreeExprBuilder::MakeExpression(if_node, field_result);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  // Create a row-batch with some sample data
  int num_records = 4;
  auto array0 = MakeArrowArrayInt32({10, 12, -20, 5}, {true, true, true, false});
  auto array1 = MakeArrowArrayInt32({5, 15, 15, 17}, {true, true, true, true});

  // expected output
  auto exp = MakeArrowArrayInt32({10, 15, 15, 17}, {true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestIfExpr, TestSimpleArithmetic) {
  // schema for input fields
  auto fielda = field("a", int32());
  auto fieldb = field("b", int32());
  auto schema = arrow::schema({fielda, fieldb});

  // output fields
  auto field_result = field("res", int32());

  // build expression.
  // if (a > b)
  //   a + b
  // else
  //   a - b
  auto node_a = TreeExprBuilder::MakeField(fielda);
  auto node_b = TreeExprBuilder::MakeField(fieldb);
  auto condition =
      TreeExprBuilder::MakeFunction("greater_than", {node_a, node_b}, boolean());
  auto sum = TreeExprBuilder::MakeFunction("add", {node_a, node_b}, int32());
  auto sub = TreeExprBuilder::MakeFunction("subtract", {node_a, node_b}, int32());
  auto if_node = TreeExprBuilder::MakeIf(condition, sum, sub, int32());

  auto expr = TreeExprBuilder::MakeExpression(if_node, field_result);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  // Create a row-batch with some sample data
  int num_records = 4;
  auto array0 = MakeArrowArrayInt32({10, 12, -20, 5}, {true, true, true, false});
  auto array1 = MakeArrowArrayInt32({5, 15, 15, 17}, {true, true, true, true});

  // expected output
  auto exp = MakeArrowArrayInt32({15, -3, -35, 0}, {true, true, true, false});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestIfExpr, TestNested) {
  // schema for input fields
  auto fielda = field("a", int32());
  auto fieldb = field("b", int32());
  auto schema = arrow::schema({fielda, fieldb});

  // output fields
  auto field_result = field("res", int32());

  // build expression.
  // if (a > b)
  //   a + b
  // else if (a < b)
  //   a - b
  // else
  //   a * b
  auto node_a = TreeExprBuilder::MakeField(fielda);
  auto node_b = TreeExprBuilder::MakeField(fieldb);
  auto condition_gt =
      TreeExprBuilder::MakeFunction("greater_than", {node_a, node_b}, boolean());
  auto condition_lt =
      TreeExprBuilder::MakeFunction("less_than", {node_a, node_b}, boolean());
  auto sum = TreeExprBuilder::MakeFunction("add", {node_a, node_b}, int32());
  auto sub = TreeExprBuilder::MakeFunction("subtract", {node_a, node_b}, int32());
  auto mult = TreeExprBuilder::MakeFunction("multiply", {node_a, node_b}, int32());
  auto else_node = TreeExprBuilder::MakeIf(condition_lt, sub, mult, int32());
  auto if_node = TreeExprBuilder::MakeIf(condition_gt, sum, else_node, int32());

  auto expr = TreeExprBuilder::MakeExpression(if_node, field_result);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  // Create a row-batch with some sample data
  int num_records = 4;
  auto array0 = MakeArrowArrayInt32({10, 12, 15, 5}, {true, true, true, false});
  auto array1 = MakeArrowArrayInt32({5, 15, 15, 17}, {true, true, true, true});

  // expected output
  auto exp = MakeArrowArrayInt32({15, -3, 225, 0}, {true, true, true, false});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0, array1});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestIfExpr, TestNestedInIf) {
  // schema for input fields
  auto fielda = field("a", int32());
  auto fieldb = field("b", int32());
  auto fieldc = field("c", int32());
  auto schema = arrow::schema({fielda, fieldb, fieldc});

  // output fields
  auto field_result = field("res", int32());

  // build expression.
  // if (a > 10)
  //   if (a < 20)
  //     a + b
  //   else
  //     b + c
  // else
  //   a + c
  auto node_a = TreeExprBuilder::MakeField(fielda);
  auto node_b = TreeExprBuilder::MakeField(fieldb);
  auto node_c = TreeExprBuilder::MakeField(fieldc);

  auto literal_10 = TreeExprBuilder::MakeLiteral(10);
  auto literal_20 = TreeExprBuilder::MakeLiteral(20);

  auto gt_10 =
      TreeExprBuilder::MakeFunction("greater_than", {node_a, literal_10}, boolean());
  auto lt_20 =
      TreeExprBuilder::MakeFunction("less_than", {node_a, literal_20}, boolean());
  auto sum_ab = TreeExprBuilder::MakeFunction("add", {node_a, node_b}, int32());
  auto sum_bc = TreeExprBuilder::MakeFunction("add", {node_b, node_c}, int32());
  auto sum_ac = TreeExprBuilder::MakeFunction("add", {node_a, node_c}, int32());

  auto if_lt_20 = TreeExprBuilder::MakeIf(lt_20, sum_ab, sum_bc, int32());
  auto if_gt_10 = TreeExprBuilder::MakeIf(gt_10, if_lt_20, sum_ac, int32());

  auto expr = TreeExprBuilder::MakeExpression(if_gt_10, field_result);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  // Create a row-batch with some sample data
  int num_records = 6;
  auto array_a =
      MakeArrowArrayInt32({21, 15, 5, 22, 15, 5}, {true, true, true, true, true, true});
  auto array_b = MakeArrowArrayInt32({20, 18, 19, 20, 18, 19},
                                     {true, true, true, false, false, false});
  auto array_c = MakeArrowArrayInt32({35, 45, 55, 35, 45, 55},
                                     {true, true, true, false, false, false});

  // expected output
  auto exp =
      MakeArrowArrayInt32({55, 33, 60, 0, 0, 0}, {true, true, true, false, false, false});

  // prepare input record batch
  auto in_batch =
      arrow::RecordBatch::Make(schema, num_records, {array_a, array_b, array_c});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestIfExpr, TestNestedInCondition) {
  // schema for input fields
  auto fielda = field("a", int32());
  auto fieldb = field("b", int32());
  auto schema = arrow::schema({fielda, fieldb});

  // output fields
  auto field_result = field("res", int32());

  // build expression.
  // if (if (a > b) then true else if (a < b) false else null)
  //   1
  // else if !(if (a > b) then true else if (a < b) false else null)
  //   2
  // else
  //   3
  auto node_a = TreeExprBuilder::MakeField(fielda);
  auto node_b = TreeExprBuilder::MakeField(fieldb);
  auto literal_1 = TreeExprBuilder::MakeLiteral(1);
  auto literal_2 = TreeExprBuilder::MakeLiteral(2);
  auto literal_3 = TreeExprBuilder::MakeLiteral(3);
  auto literal_true = TreeExprBuilder::MakeLiteral(true);
  auto literal_false = TreeExprBuilder::MakeLiteral(false);
  auto literal_null = TreeExprBuilder::MakeNull(boolean());

  auto a_gt_b =
      TreeExprBuilder::MakeFunction("greater_than", {node_a, node_b}, boolean());
  auto a_lt_b = TreeExprBuilder::MakeFunction("less_than", {node_a, node_b}, boolean());
  auto cond_else =
      TreeExprBuilder::MakeIf(a_lt_b, literal_false, literal_null, boolean());
  auto cond_if = TreeExprBuilder::MakeIf(a_gt_b, literal_true, cond_else, boolean());
  auto not_cond_if = TreeExprBuilder::MakeFunction("not", {cond_if}, boolean());

  auto outer_else = TreeExprBuilder::MakeIf(not_cond_if, literal_2, literal_3, int32());
  auto outer_if = TreeExprBuilder::MakeIf(cond_if, literal_1, outer_else, int32());
  auto expr = TreeExprBuilder::MakeExpression(outer_if, field_result);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  // Create a row-batch with some sample data
  int num_records = 6;
  auto array_a =
      MakeArrowArrayInt32({21, 15, 5, 22, 15, 5}, {true, true, true, true, true, true});
  auto array_b = MakeArrowArrayInt32({20, 18, 19, 20, 18, 19},
                                     {true, true, true, false, false, false});
  // expected output
  auto exp =
      MakeArrowArrayInt32({1, 2, 2, 3, 3, 3}, {true, true, true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array_a, array_b});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestIfExpr, TestBigNested) {
  // schema for input fields
  auto fielda = field("a", int32());
  auto schema = arrow::schema({fielda});

  // output fields
  auto field_result = field("res", int32());

  // build expression.
  // if (a < 10)
  //   10
  // else if (a < 20)
  //   20
  // ..
  // ..
  // else if (a < 190)
  //   190
  // else
  //   200
  auto node_a = TreeExprBuilder::MakeField(fielda);
  auto top_node = TreeExprBuilder::MakeLiteral(200);
  for (int thresh = 190; thresh > 0; thresh -= 10) {
    auto literal = TreeExprBuilder::MakeLiteral(thresh);
    auto condition =
        TreeExprBuilder::MakeFunction("less_than", {node_a, literal}, boolean());
    auto if_node = TreeExprBuilder::MakeIf(condition, literal, top_node, int32());
    top_node = if_node;
  }
  auto expr = TreeExprBuilder::MakeExpression(top_node, field_result);

  // Build a projector for the expressions.
  std::shared_ptr<Projector> projector;
  auto status = Projector::Make(schema, {expr}, TestConfiguration(), &projector);
  EXPECT_TRUE(status.ok());

  // Create a row-batch with some sample data
  int num_records = 4;
  auto array0 = MakeArrowArrayInt32({10, 102, 158, 302}, {true, true, true, true});

  // expected output
  auto exp = MakeArrowArrayInt32({20, 110, 160, 200}, {true, true, true, true});

  // prepare input record batch
  auto in_batch = arrow::RecordBatch::Make(schema, num_records, {array0});

  // Evaluate expression
  arrow::ArrayVector outputs;
  status = projector->Evaluate(*in_batch, pool_, &outputs);
  EXPECT_TRUE(status.ok());

  // Validate results
  EXPECT_ARROW_ARRAY_EQUALS(exp, outputs.at(0));
}

TEST_F(TestIfExpr, FlatCaseOrderNullsAndNestedBranches) {
  auto input = field("a", int32());
  auto schema = arrow::schema({input});
  auto a = TreeExprBuilder::MakeField(input);
  auto positive = TreeExprBuilder::MakeFunction(
      "greater_than", {a, TreeExprBuilder::MakeLiteral(int32_t{0})}, boolean());
  auto nested = TreeExprBuilder::MakeIf(
      positive, a, TreeExprBuilder::MakeLiteral(int32_t{-1}), int32());
  auto null_value = TreeExprBuilder::MakeNull(int32());
  auto node = TreeExprBuilder::MakeCase(
      {TreeExprBuilder::MakeNull(boolean()), positive,
       TreeExprBuilder::MakeLiteral(true)},
      {TreeExprBuilder::MakeLiteral(int32_t{99}), nested, null_value},
      TreeExprBuilder::MakeLiteral(int32_t{88}), int32());
  auto baseline = TreeExprBuilder::MakeIf(
      TreeExprBuilder::MakeNull(boolean()), TreeExprBuilder::MakeLiteral(int32_t{99}),
      TreeExprBuilder::MakeIf(positive, nested, null_value, int32()), int32());
  for (bool optimize : {false, true}) {
    auto config = ConfigurationBuilder().build(optimize);
    std::shared_ptr<Projector> projector;
    ASSERT_OK(Projector::Make(
        schema,
        {TreeExprBuilder::MakeExpression(node, field("flat", int32())),
         TreeExprBuilder::MakeExpression(baseline, field("nested", int32()))},
        config, &projector));
    auto batch = arrow::RecordBatch::Make(
        schema, 4, {MakeArrowArrayInt32({10, -1, 0, 5}, {true, true, true, false})});
    arrow::ArrayVector outputs;
    ASSERT_OK(projector->Evaluate(*batch, pool_, &outputs));
    EXPECT_ARROW_ARRAY_EQUALS(outputs[0], outputs[1]);
    EXPECT_ARROW_ARRAY_EQUALS(
        MakeArrowArrayInt32({10, 0, 0, 0}, {true, false, false, false}), outputs[0]);
  }
}

TEST_F(TestIfExpr, FlatCaseShortCircuitsConditionsAndResults) {
  auto input = field("a", int32());
  auto schema = arrow::schema({input});
  auto a = TreeExprBuilder::MakeField(input);
  auto divide = TreeExprBuilder::MakeFunction(
      "divide", {a, TreeExprBuilder::MakeLiteral(int32_t{0})}, int32());
  auto throwing_condition =
      TreeExprBuilder::MakeFunction("equal", {divide, a}, boolean());
  auto node =
      TreeExprBuilder::MakeCase({TreeExprBuilder::MakeLiteral(false),
                                 TreeExprBuilder::MakeLiteral(true), throwing_condition},
                                {divide, a, divide}, divide, int32());
  for (bool optimize : {false, true}) {
    std::shared_ptr<Projector> projector;
    ASSERT_OK(Projector::Make(
        schema, {TreeExprBuilder::MakeExpression(node, field("out", int32()))},
        ConfigurationBuilder().build(optimize), &projector));
    auto array = MakeArrowArrayInt32({10, -1, 0, 5}, {true, true, true, false});
    auto batch = arrow::RecordBatch::Make(schema, 4, {array});
    arrow::ArrayVector outputs;
    ASSERT_OK(projector->Evaluate(*batch, pool_, &outputs));
    EXPECT_ARROW_ARRAY_EQUALS(array, outputs[0]);
  }
}

TEST_F(TestIfExpr, FlatCaseLarge) {
  auto input = field("a", int32());
  auto schema = arrow::schema({input});
  auto a = TreeExprBuilder::MakeField(input);
  NodeVector conditions;
  NodeVector results;
  constexpr int32_t branches = 5000;
  for (int32_t i = 0; i < branches; ++i) {
    conditions.push_back(TreeExprBuilder::MakeFunction(
        "equal", {a, TreeExprBuilder::MakeLiteral(i)}, boolean()));
    results.push_back(TreeExprBuilder::MakeLiteral(i + 10));
  }
  auto node = TreeExprBuilder::MakeCase(conditions, results,
                                        TreeExprBuilder::MakeNull(int32()), int32());
  for (bool optimize : {false, true}) {
    std::shared_ptr<Projector> projector;
    ASSERT_OK(Projector::Make(
        schema, {TreeExprBuilder::MakeExpression(node, field("out", int32()))},
        ConfigurationBuilder().build(optimize), &projector));
    auto batch = arrow::RecordBatch::Make(
        schema, 5,
        {MakeArrowArrayInt32({0, 2500, branches - 1, branches, 0},
                             {true, true, true, true, false})});
    arrow::ArrayVector outputs;
    ASSERT_OK(projector->Evaluate(*batch, pool_, &outputs));
    EXPECT_ARROW_ARRAY_EQUALS(MakeArrowArrayInt32({10, 2510, branches + 9, 0, 0},
                                                  {true, true, true, false, false}),
                              outputs[0]);
  }
}

TEST_F(TestIfExpr, FlatCaseStringAndDecimal) {
  for (const auto& type : {arrow::utf8(), arrow::decimal128(20, 2)}) {
    auto condition_field = field("condition", boolean());
    auto then_field = field("then", type);
    auto else_field = field("else", type);
    auto schema = arrow::schema({condition_field, then_field, else_field});
    auto condition = TreeExprBuilder::MakeField(condition_field);
    auto then_node = TreeExprBuilder::MakeField(then_field);
    auto else_node = TreeExprBuilder::MakeField(else_field);
    auto inner = TreeExprBuilder::MakeCase({condition}, {then_node}, else_node, type);
    auto node =
        TreeExprBuilder::MakeCase({condition, TreeExprBuilder::MakeNull(boolean())},
                                  {inner, then_node}, else_node, type);
    auto baseline = TreeExprBuilder::MakeIf(condition, then_node, else_node, type);
    auto conditions =
        MakeArrowArrayBool({true, false, false, true}, {true, true, false, true});
    auto then_array =
        arrow::ArrayFromJSON(type, "[\"123.45\", \"-54.32\", \"12.00\", null]");
    auto else_array = arrow::ArrayFromJSON(type, "[\"0.00\", \"98.76\", null, \"1.00\"]");
    auto expected = arrow::ArrayFromJSON(type, "[\"123.45\", \"98.76\", null, null]");
    for (bool optimize : {false, true}) {
      std::shared_ptr<Projector> projector;
      ASSERT_OK(Projector::Make(
          schema,
          {TreeExprBuilder::MakeExpression(node, field("flat", type)),
           TreeExprBuilder::MakeExpression(baseline, field("nested", type))},
          ConfigurationBuilder().build(optimize), &projector));
      auto batch =
          arrow::RecordBatch::Make(schema, 4, {conditions, then_array, else_array});
      arrow::ArrayVector outputs;
      ASSERT_OK(projector->Evaluate(*batch, pool_, &outputs));
      EXPECT_ARROW_ARRAY_EQUALS(expected, outputs[0]);
      EXPECT_ARROW_ARRAY_EQUALS(outputs[1], outputs[0]);
    }
  }
}

TEST_F(TestIfExpr, FlatCaseHasOneMerge) {
  auto input = field("a", int32());
  auto schema = arrow::schema({input});
  auto a = TreeExprBuilder::MakeField(input);
  NodeVector conditions;
  NodeVector results;
  for (int32_t i = 0; i < 10; ++i) {
    conditions.push_back(TreeExprBuilder::MakeFunction(
        "equal", {a, TreeExprBuilder::MakeLiteral(i)}, boolean()));
    results.push_back(TreeExprBuilder::MakeLiteral(i + 1));
  }
  auto node = TreeExprBuilder::MakeCase(conditions, results, a, int32());
  auto config = ConfigurationBuilder().build(false);
  config->set_dump_ir(true);
  std::shared_ptr<Projector> projector;
  ASSERT_OK(Projector::Make(
      schema, {TreeExprBuilder::MakeExpression(node, field("out", int32()))}, config,
      &projector));
  const auto& ir = projector->DumpIR();
  auto phi = ir.find("case.value = phi i32");
  ASSERT_NE(std::string::npos, phi);
  EXPECT_EQ(std::string::npos, ir.find("case.value = phi i32", phi + 1));
  EXPECT_EQ(std::string::npos, ir.find("res_value = phi"));
}

}  // namespace gandiva
