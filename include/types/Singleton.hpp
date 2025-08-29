#pragma once

namespace sb::types {

template <class T>
class Singleton {
 public:
  static auto Instance() -> T& {
    static T instance;
    return instance;
  }

 protected:
  Singleton() = default;
  ~Singleton() = default;

 public:
  Singleton(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  auto operator=(const Singleton&) -> Singleton& = delete;
  auto operator=(Singleton&&) -> Singleton& = delete;
};
}  // namespace sb::types
