#include "include/logger.hpp"
#include "include/task_queue.hpp"
#include <iostream>
#include <thread>

int main() {
  TaskQueue pool(4);

  // Task 1: Simple computation (returns int)
  auto f1 = pool.enqueue([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 42;
  });

  // Task 2: String concatenation (returns std::string)
  auto f2 = pool.enqueue([] { return std::string("Hello from task 2"); });

  // Task 3: Print message (void return)
  pool.enqueue([] { Logger::log("Task 3: Doing some logging..."); });

  // Task 4: Heavy computation
  auto f4 = pool.enqueue([] {
    long sum = 0;
    for (int i = 0; i < 1e6; ++i)
      sum += i;
    return sum;
  });

  // Collect results
  Logger::log("Task 1 result: ", f1.get());
  Logger::log("Task 2 result: ", f2.get());
  Logger::log("Task 4 result: ", f4.get());

  return 0;
}
