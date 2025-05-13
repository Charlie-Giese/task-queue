#include "../include/task_queue.hpp"
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void simulate_work(int iterations = 1000) {
  volatile int x = 0;
  for (int i = 0; i < iterations; ++i)
    x += i % 13;
}

void run_scalability_test(size_t num_threads, size_t num_tasks) {
  TaskQueue queue(num_threads);

  std::mutex mutex;
  std::condition_variable cv;
  size_t completed = 0;

  auto start_time = std::chrono::steady_clock::now();

  for (size_t i = 0; i < num_tasks; ++i) {
    queue.enqueue([&]() {
      simulate_work(2000); // Simulate some CPU work
      {
        std::lock_guard<std::mutex> lock(mutex);
        completed++;
      }
      cv.notify_one();
    });
  }

  {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&]() { return completed == num_tasks; });
  }

  auto end_time = std::chrono::steady_clock::now();
  double elapsed_s =
      std::chrono::duration<double>(end_time - start_time).count();

  double throughput = static_cast<double>(num_tasks) / elapsed_s;
  std::cout << "Threads: " << num_threads << ", Time: " << elapsed_s << " s"
            << ", Throughput: " << throughput << " tasks/s\n";
}

int main() {
  const size_t num_tasks = 10000;

  std::cout << "=== Scalability Benchmark ===\n";

  for (size_t threads = 1; threads <= std::thread::hardware_concurrency();
       threads *= 2) {
    run_scalability_test(threads, num_tasks);
  }

  return 0;
}
