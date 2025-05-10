#include "../include/task_queue.hpp"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

TEST(TaskQueueTest, EnqueuesAndExecutesTask) {
  TaskQueue queue(4);
  std::atomic<bool> flag{false};

  queue.enqueue([&]() { flag = true; });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_TRUE(flag);
}
