#pragma once

namespace sb::types {

template <class T>
class Singleton {
 public:
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

  auto static Instance() -> T & {
    if (!instance_) instance_ = new T_Instance;
    return *instance_;
  }

 protected:
  Singleton() {}
  ~Singleton() { delete instance_; }

 private:
  struct T_Instance : public T {
    T_Instance() : T() {}
  };

  static inline T *instance_ = nullptr;
};
}  // namespace sb::types
