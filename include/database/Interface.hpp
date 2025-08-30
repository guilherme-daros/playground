#pragma once

#include <list>
#include <optional>
#include <string>

namespace sb::database {

template <class T, typename PrimaryKeyType = std::string>
class Interface {
 public:
  virtual ~Interface() = default;
  virtual auto Create(const T& obj) -> bool = 0;
  virtual auto Read(const PrimaryKeyType& key) -> std::optional<T> = 0;
  virtual auto ReadAll() -> std::list<T> = 0;
  virtual auto Update(const T& obj) -> bool = 0;
  virtual auto Delete(const PrimaryKeyType& key) -> bool = 0;
  virtual auto DeleteAll() -> bool = 0;
};

}  // namespace sb::database
