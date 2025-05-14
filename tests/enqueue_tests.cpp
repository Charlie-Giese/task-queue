#include "../include/task_queue.hpp"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

// Ensure enqueue method works properly
TEST(TaskQueueTest, EnqueuesAndExecutesTask) {
  TaskQueue queue(4);
  std::atomic<bool> flag{false};

  queue.enqueue([&]() { flag = true; });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_TRUE(flag);
}

// Ensure that tasks are completed in the correct order (1 thread)
TEST(TaskQueueTest, QueueOrder) {

  TaskQueue queue(1);
  std::vector<int> vec;
  std::mutex mutex;

  for (int i = 1; i <= 5; i++) {
    queue.enqueue([&, i]() {
      std::lock_guard<std::mutex> lock(mutex);
      vec.push_back(i);
    });
  }

  std::vector<int> expected_order = {1, 2, 3, 4, 5};

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(vec, expected_order);
}
