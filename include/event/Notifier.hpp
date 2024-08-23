#pragma once

#include <typeinfo>

#include "Listener.hpp"
namespace sb::event {

template <typename L, typename T>
auto AddListener(T* listener) -> void {
  auto& list = Listener<T>::notify_list();
  list.insert({typeid(L).hash_code(), listener});
}

template <typename L, typename T>
auto RemoveListener(T* listener) -> void {
  auto& list = Listener<T>::notify_list();
  list.erase(typeid(L).hash_code());
}

template <typename T, typename... Args>
  requires std::invocable<T, Args...>
auto Notify(Args... args) -> void {
  Listener<T>::Notify(args...);
}
}  // namespace sb::event
