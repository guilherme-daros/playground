#pragma once

#include <list>
#include <optional>

namespace sb::database {

template <class T>
class Interface {
 public:
  virtual auto Create(T& obj) -> bool = 0;
  virtual auto Read(T& obj) -> std::optional<T> = 0;
  virtual auto ReadAll(T& obj) -> std::list<T> = 0;
  virtual auto Update(T& obj) -> bool = 0;
  virtual auto Delete(T& obj) -> void = 0;
  virtual auto DeleteAll(T& obj) -> void = 0;
};

}  // namespace sb::database
