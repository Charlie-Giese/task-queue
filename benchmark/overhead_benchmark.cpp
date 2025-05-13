#include "../include/task_queue.hpp"
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>

int main() {
  const size_t num_tasks = 100000;
  TaskQueue queue(std::thread::hardware_concurrency());

  std::mutex mutex;
  std::condition_variable cv;
  size_t completed = 0;

  auto start = std::chrono::steady_clock::now();

  for (size_t i = 0; i < num_tasks; ++i) {
    queue.enqueue([&]() {
      {
        std::lock_guard<std::mutex> lock(mutex);
        completed++;
      }
      cv.notify_one(); // Almost no work — measure scheduler overhead
    });
  }

  {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&]() { return completed == num_tasks; });
  }

  auto end = std::chrono::steady_clock::now();

  double total_us =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();
  double avg_us = total_us / num_tasks;

  std::cout << "Overhead benchmark:\n";
  std::cout << "Total time: " << total_us << " µs\n";
  std::cout << "Average task latency (enqueue + dispatch): " << avg_us
            << " µs\n";

  return 0;
}
