#include "../include/task_queue.hpp"
#include <atomic>
#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <vector>

TEST(TaskQueueTest, HandlesMultipleTasks) {
  TaskQueue queue(10);
  std::atomic<int> sum{0};

  for (int i = 1; i <= 5; ++i) {
    queue.enqueue([&, i]() { sum += i; });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  EXPECT_EQ(sum, 15); // 1 + 2 + 3 + 4 + 5
}
