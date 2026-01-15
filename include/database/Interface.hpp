#pragma once

#include <list>
#include <optional>
#include <string>

#include "types/Result.hpp"

namespace sb::database {

template <class T, typename PrimaryKeyType = std::string>
class Interface {
 public:
  virtual ~Interface() = default;
  virtual auto Create(const T& obj) -> Result<void> = 0;
  virtual auto Read(const PrimaryKeyType& key) -> Result<std::optional<T>> = 0;
  virtual auto ReadAll() -> Result<std::list<T>> = 0;
  virtual auto Update(const T& obj) -> Result<void> = 0;
  virtual auto Delete(const PrimaryKeyType& key) -> Result<void> = 0;
  virtual auto DeleteAll() -> Result<void> = 0;
};

}  // namespace sb::database
