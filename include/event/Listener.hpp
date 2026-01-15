#pragma once

#include <cstdint>
#include <exception>
#include <future>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "logger/Logger.hpp"
#include "types/ThreadPool.hpp"
#include "types/Typename.hpp"

namespace sb::event {

class Notifier;

template <typename T>
class Listener {
  static constexpr auto kName = sb::types::Typename<T>::get();

  using List = std::unordered_map<std::uintptr_t, T*>;
  using EventLogger = sb::logger::Logger<kName>;

 public:
  static auto Notify(auto... args) -> void {
    std::lock_guard<std::mutex> lock(mtx_);

    sb::types::ThreadPool::SetConfig(2);
    auto& tp = sb::types::ThreadPool::Instance();

    EventLogger::logging_level = logger::Level::Debug;

    typename EventLogger::Debug() << "Notifying";
    for (auto const& [id, listener] : notify_list()) {
      futures_.push_back(tp.enqueue([listener, args...]() {
        try {
          (*listener)(args...);
        } catch (const std::exception& e) {
          typename EventLogger::Error() << e.what();
        } catch (...) {
          typename EventLogger::Error() << "Unknown exception caught";
        }
      }));
    }
  }

  static auto NotifySync(auto... args) -> void {
    for (auto const& [id, listener] : notify_list()) {
      try {
        (*listener)(args...);
      } catch (const std::exception& e) {
        typename EventLogger::Error() << e.what();
      } catch (...) {
        typename EventLogger::Error() << "Unknown exception caught";
      }
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
        try {
          f.get();
        } catch (const std::exception& e) {
          typename EventLogger::Error() << "Thrown: " << e.what();
        } catch (...) {
          typename EventLogger::Error() << "Unknown exception caught";
        }
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
