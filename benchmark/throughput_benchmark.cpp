#include "../include/task_queue.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  const size_t num_tasks = 1'000'000;
  const size_t num_threads = std::thread::hardware_concurrency();

  std::cout << "Running throughput benchmark with " << num_threads
            << " threads...\n";

  std::atomic<size_t> counter{0};

  TaskQueue queue(num_threads);

  auto start = std::chrono::steady_clock::now();

  for (size_t i = 0; i < num_tasks; i++) {
    queue.enqueue(
        [&counter]() { counter.fetch_add(1, std::memory_order_relaxed); });
  }

  while (counter.load(std::memory_order_relaxed) < num_tasks) {
    std::this_thread::yield();
  }

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  double throughput = num_tasks / elapsed.count();

  std::cout << "Completed " << num_tasks << " tasks in " << elapsed.count()
            << " seconds";
  std::cout << "Throughput: " << throughput << " tasks/sec\n";

  return 0;
}
