#pragma once

#include "Listener.hpp"

namespace sb::event {

template <typename ListenerType, typename EventType>
auto AddListener(ListenerType* listener) -> void {
  auto& list = Listener<EventType>::notify_list();
  list.insert({reinterpret_cast<std::uintptr_t>(listener), static_cast<EventType*>(listener)});
}

template <typename ListenerType, typename EventType>
auto RemoveListener(ListenerType* listener) -> void {
  auto& list = Listener<EventType>::notify_list();
  list.erase(reinterpret_cast<std::uintptr_t>(listener));
}

template <typename T, typename... Args>
  requires std::invocable<T, Args...>
auto Notify(Args... args) -> void {
  Listener<T>::Notify(args...);
}
}  // namespace sb::event
