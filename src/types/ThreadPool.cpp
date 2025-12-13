#include "ThreadPool.hpp"

namespace sb::types {

size_t ThreadPool::thread_count_ = std::thread::hardware_concurrency();

void ThreadPool::SetConfig(size_t count) { thread_count_ = count; }

ThreadPool::ThreadPool() : stop(false) {
  for (size_t i = 0; i < thread_count_; ++i)
    workers.emplace_back([this] {
      for (;;) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->queue_mutex);

          this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

          if (this->stop && this->tasks.empty()) {
            return;
          }

          task = std::move(this->tasks.front());
          this->tasks.pop();
        }

        task();
      }
    });
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }

  condition.notify_all();

  for (auto &worker : workers) {
    worker.join();
  };
}

}  // namespace sb::types
