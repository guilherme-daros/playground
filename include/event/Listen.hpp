#pragma once

#include "Notifier.hpp"

namespace sb::event {
template <typename T, typename... Ts>
class Listen : public Ts... {
 protected:
  explicit Listen(T* l) { (AddListener<T, Ts>(l), ...); }

  ~Listen() { (RemoveListener<T, Ts>(static_cast<T*>(this)), ...); }

  static auto Wait() { (Listener<Ts>::Wait(), ...); }
};
}  // namespace sb::event
