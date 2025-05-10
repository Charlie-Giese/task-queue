#include "../include/task_queue.hpp"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

TEST(TaskQueueTest, EnqueueAndExecuteTasks) {
  TaskQueue queue(4);
  std::atomic<int> counter = 0;

  queue.enqueue([&] { counter += 1; });
  queue.enqueue([&] { counter += 2; });
  queue.enqueue([&] { counter += 3; });

  // Give some time for tasks to finish
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  EXPECT_EQ(counter, 6);
}
