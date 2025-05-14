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
  // Constructor, constructor
  explicit TaskQueue(size_t num_threads);
  ~TaskQueue();

  // Enqueue template declaration
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

// Enqueue template implementation
template <typename Func, typename... Args>
auto TaskQueue::enqueue(Func &&f, Args &&...args)
    -> std::future<typename std::invoke_result_t<Func, Args...>> {

  // getting return type for later
  using return_type = std::invoke_result_t<Func, Args...>;

  // package callable with bound args to get future later
  auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

  // get future from task
  std::future<return_type> res = task_ptr->get_future();

  // lock queue, put new task in queue if not stopped
  {
    std::lock_guard<std::mutex> lock(queue_mutex);

    if (stop) {
      throw std::runtime_error("Enqueue on stopped TaskQueue");
    }

    tasks.emplace([task_ptr]() { (*task_ptr)(); });
  }
  // Wake up thread
  condition.notify_one();
  return res;
}

#endif // TASK_QUEUE_HPP
