#include "../include/task_queue.hpp"
#include <functional>
#include <mutex>

TaskQueue::TaskQueue(size_t num_threads) {
  for (size_t i = 0; i < num_threads; i++) {
    // Launc num_threads threads with lambda that captures object for member
    // access
    workers.emplace_back([this] {
      // endless loop so that thread waits endlessly for new tasks
      for (;;) {
        std::function<void()> task;
        // Get lock on queue, wait until new tasks are there or pool is stopped
        {
          std::unique_lock<std::mutex> lock(this->queue_mutex);
          this->condition.wait(
              lock, [this] { return this->stop || !this->tasks.empty(); });
          // if stopped, and no new tasks, end thread
          if (this->stop && this->tasks.empty())
            return;
          // get tasks from queue and delete from queue
          task = std::move(this->tasks.front());
          this->tasks.pop();
        }

        task(); // execute the task
      }
    });
  }
}

TaskQueue::~TaskQueue() {
  // get lock and set stop to true
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  // wake all threads so they can shut down
  condition.notify_all();

  for (std::thread &worker : workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}
