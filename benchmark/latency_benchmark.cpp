#include "../include/task_queue.hpp"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

void simulate_cpu_work(int iterations = 1000) {
  volatile int x = 0;
  for (int i = 0; i < iterations; ++i) {
    x += i % 7;
  }
}

int main() {
  const size_t num_tasks = 10000;
  const size_t num_threads = std::thread::hardware_concurrency();

  std::cout << "Running latency benchmark with " << num_threads
            << " threads...\n";

  TaskQueue queue(num_threads);

  std::vector<double> latencies;
  latencies.reserve(num_tasks);

  std::mutex data_mutex;
  std::condition_variable cv;
  size_t completed = 0;

  for (size_t i = 0; i < num_tasks; ++i) {
    auto enqueue_time = std::chrono::steady_clock::now();

    queue.enqueue([enqueue_time, &latencies, &data_mutex, &cv, &completed]() {
      auto exec_time = std::chrono::steady_clock::now();
      double latency_us = std::chrono::duration_cast<std::chrono::microseconds>(
                              exec_time - enqueue_time)
                              .count();

      simulate_cpu_work(2000); // Simulate CPU-bound task

      {
        std::lock_guard<std::mutex> lock(data_mutex);
        latencies.push_back(latency_us);
        completed++;
      }
      cv.notify_one();
    });
  }

  // Wait for all tasks to complete
  {
    std::unique_lock<std::mutex> lock(data_mutex);
    cv.wait(lock, [&completed, num_tasks] { return completed == num_tasks; });
  }

  // Sort latencies for percentile stats
  std::sort(latencies.begin(), latencies.end());

  auto percentile = [&](double p) -> double {
    size_t index = static_cast<size_t>(p * latencies.size());
    index = std::min(index, latencies.size() - 1);
    return latencies[index];
  };

  double avg_latency =
      std::accumulate(latencies.begin(), latencies.end(), 0.0) /
      latencies.size();

  std::cout << "Tasks completed: " << latencies.size() << "\n";
  std::cout << "Average latency: " << avg_latency << " µs\n";
  std::cout << "Min latency: " << latencies.front() << " µs\n";
  std::cout << "Max latency: " << latencies.back() << " µs\n";
  std::cout << "Median (P50): " << percentile(0.50) << " µs\n";
  std::cout << "P95 latency: " << percentile(0.95) << " µs\n";
  std::cout << "P99 latency: " << percentile(0.99) << " µs\n";

  return 0;
}
