#pragma once

#include <functional>
#include "Notifier.hpp"

namespace sb::event {
template <typename... Ts>
class Listen : public Ts... {
 protected:
  template <typename Derived>
  explicit Listen(Derived* derived) {
    (AddListener<Derived, Ts>(derived), ...);
    unregister_ = [derived] { (RemoveListener<Derived, Ts>(derived), ...); };
  }

  ~Listen() {
    if (unregister_) {
      unregister_();
    }
  }

  static auto Wait() { (Listener<Ts>::Wait(), ...); }

 private:
  std::function<void()> unregister_;
};
}  // namespace sb::event
