#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <vector>

class TaskQueue {
public:
  explicit TaskQueue(size_t num_threads);
  ~TaskQueue();

  template <typename F, typename... Args>
  auto enqueue(F &&f, Args &&...args)
      -> std::future<typename std::invoke_result_t<F, Args...>>;

  // Disable copy and assignment. TODO: implement move constructor and move
  // assignment operators
  TaskQueue(const TaskQueue &) = delete;
  TaskQueue &operator=(const TaskQueue &) = delete;

private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex queue_mutex;
  std::condition_variable condition;
  std::atomic<bool> stop = false;
};

template <typename Func, typename... Args>
auto TaskQueue::enqueue(Func &&f, Args &&...args)
    -> std::future<typename std::invoke_result_t<Func, Args...>> {

  using return_type = std::invoke_result_t<Func, Args...>;

  auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task_ptr->get_future();

  {
    std::lock_guard<std::mutex> lock(queue_mutex);

    if (stop) {
      throw std::runtime_error("Enqueue on stopped TaskQueue");
    }

    tasks.emplace([task_ptr]() { (*task_ptr)(); });
  }

  condition.notify_one();
  return res;
}

#endif // TASK_QUEUE_HPP
