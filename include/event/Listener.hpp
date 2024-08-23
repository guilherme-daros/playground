#pragma once

#include <future>
#include <unordered_map>

namespace sb::event {

class Notifier;

template <typename T>
class Listener {
  using List = std::unordered_map<std::size_t, T*>;

 public:
  static auto Notify(auto... args) -> void {
    auto run = [](auto... args) -> void {
      for (auto listener : notify_list()) {
        (*std::get<1>(listener))(args...);
      }
    };

    future_ = std::async(std::launch::async, run, args...);
  }

  static auto Wait() -> void {
    if (future_.valid()) {
      future_.wait();
    }
  }

  static auto notify_list() -> List& { return notify_list_; }

 private:
  static std::future<void> future_;
  static List notify_list_;
};

template <typename T>
Listener<T>::List Listener<T>::notify_list_;

template <typename T>
std::future<void> Listener<T>::future_;

}  // namespace sb::event
