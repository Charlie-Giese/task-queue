#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class TaskQueue {
public:
  explicit TaskQueue(size_t num_threads);
  ~TaskQueue();

  template <typename F, typename... Args>
  auto enqueue(F &&f, Args &&...args)
      -> std::future<std::invoke_result_t<F, Args...>>;

private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex queue_mutex;
  std::condition_variable condition;
  std::atomic<bool> stop;

  void worker_loop();
};
