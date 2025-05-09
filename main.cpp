#include "include/logger.hpp"
#include "include/task_queue.hpp"
#include <iostream>
#include <thread>

int main() {
  TaskQueue pool(4);

  // Example usage
  pool.enqueue([] {
    Logger::log("Task 1 is running on thread ", std::this_thread::get_id());
  });

  pool.enqueue([] {
    Logger::log("Task 2: Computing result...");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Logger::log("Task 2 done.");
  });

  // You can still return values too
  auto f = pool.enqueue([] {
    Logger::log("Task 3 running");
    return 123;
  });

  Logger::log("Task 3 result: ", f.get());

  return 0;
}
