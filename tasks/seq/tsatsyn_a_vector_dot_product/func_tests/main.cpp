// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <vector>

#include "seq/tsatsyn_a_vector_dot_product/include/ops_seq.hpp"

std::vector<int> GetRandomVector(int size) {
  std::vector<int> vector(size);
  std::srand((time(NULL)));
  for (int i = 0; i < size; ++i) {
    vector[i] = std::rand() % 100 + 1;
  }
  return vector;
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Random_Scalar) {
  const int size = 5;

  // Create data
  std::vector<int> v1 = GetRandomVector(size);
  std::vector<int> v2 = GetRandomVector(size);
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Negative_Validation) {
  // Create data
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {1, 2, 3, 4};
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), false);
  /*testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(scale, ans[0]);*/
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Scalar_32) {
  const int scale = 32;

  // Create data
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(scale, ans[0]);
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Scalar_28) {
  const int scale = 28;

  // Create data
  std::vector<int> v1 = {6, 4, 5};
  std::vector<int> v2 = {1, 3, 2};
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(scale, ans[0]);
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Scalar_95) {
  const int scale = 95;

  // Create data
  std::vector<int> v1 = {7, 4, 6};
  std::vector<int> v2 = {3, 5, 9};
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(scale, ans[0]);
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Scalar_2330) {
  const int scale = 2330;

  // Create data
  std::vector<int> v1 = {20, 54, 23};
  std::vector<int> v2 = {32, 10, 50};
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(scale, ans[0]);
}

TEST(tsatsyn_a_vector_dot_product_seq, Test_Scalar_1956) {
  const int count = 1956;

  // Create data
  std::vector<int> v1 = {12, 100, 50};
  std::vector<int> v2 = {13, 3, 30};
  std::vector<int> ans(1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v1.data()));
  taskDataSeq->inputs_count.emplace_back(v1.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(v2.data()));
  taskDataSeq->inputs_count.emplace_back(v2.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(ans.data()));
  taskDataSeq->outputs_count.emplace_back(ans.size());

  // Create Task
  tsatsyn_a_vector_dot_product_seq::TestTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(count, ans[0]);
}