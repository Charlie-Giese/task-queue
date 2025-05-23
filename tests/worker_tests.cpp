#include "../include/task_queue.hpp"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <vector>

// Ensure queue can handle multiple tasks cleanly
TEST(TaskQueueTest, HandlesMultipleTasks) {
  TaskQueue queue(10);
  std::atomic<int> sum{0};

  for (int i = 1; i <= 5; ++i) {
    queue.enqueue([&, i]() { sum += i; });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  EXPECT_EQ(sum, 15); // 1 + 2 + 3 + 4 + 5
}

// Ensure thread safety, count over multiple tasks from multiple numThreads
TEST(TaskQueueTest, ThreadSafety) {
  std::vector<std::thread> threads;

  int numThreads = 4;
  int tasksPerThread = 100;

  std::atomic<int> counter{0};

  TaskQueue queue(numThreads);

  for (int i = 0; i < numThreads; i++) {
    threads.emplace_back([&]() {
      for (int j = 0; j < tasksPerThread; j++) {
        queue.enqueue(
            [&counter]() { counter.fetch_add(1, std::memory_order_relaxed); });
      }
    });
  }

  for (auto &t : threads)
    t.join();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(counter.load(), numThreads * tasksPerThread);
}

// Ensure graceful completion of tasks after shutdown
TEST(TaskQueueTest, GracefulShutdown) {
  std::atomic<int> sum{0};
  std::promise<void> done;
  auto done_future = done.get_future();

  {
    TaskQueue queue(10);
    for (int i = 1; i <= 5; i++) {
      queue.enqueue([&, i]() {
        sum += i;
        if (i == 5) {
          done.set_value(); // Signal when the last task is done
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      });
    }
  }

  // Wait for signal that final task has completed
  done_future.wait();

  EXPECT_EQ(sum.load(), 15);
}

// Ensure thread exception doesn't block queue
TEST(TaskQueueTest, HandlesExceptionsGracefully) {
  std::atomic<bool> exception_caught{false};
  std::atomic<bool> continued_execution{false};

  {
    TaskQueue queue(2);

    // Enqueue a task that throws
    queue.enqueue([&]() { throw std::runtime_error("Test exception"); });

    // Enqueue a second task to ensure thread continues
    queue.enqueue([&]() { continued_execution = true; });

    // Give workers time to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  // Check that the second task still ran
  EXPECT_TRUE(continued_execution);
}
