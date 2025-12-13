#pragma once

#include <cstdint>
#include <future>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace sb::event {

class Notifier;

template <typename T>
class Listener {
  using List = std::unordered_map<std::uintptr_t, T*>;

 public:
  static auto Notify(auto... args) -> void {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto const& [id, listener] : notify_list()) {
      futures_.push_back(std::async(std::launch::async, [listener, args...]() { (*listener)(args...); }));
    }
  }

  static auto Wait() -> void {
    std::vector<std::future<void>> local_futures;
    {
      std::lock_guard<std::mutex> lock(mtx_);
      local_futures.swap(futures_);
    }

    for (auto& f : local_futures) {
      if (f.valid()) {
        f.wait();
      }
    }
  }

  static auto notify_list() -> List& { return notify_list_; }

 private:
  static List notify_list_;
  static std::vector<std::future<void>> futures_;
  static std::mutex mtx_;
};

template <typename T>
Listener<T>::List Listener<T>::notify_list_;

template <typename T>
std::vector<std::future<void>> Listener<T>::futures_;

template <typename T>
std::mutex Listener<T>::mtx_;

}  // namespace sb::event
