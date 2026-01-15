#pragma once
#include <exception>
#include <string>

#include "types/Typename.hpp"

namespace sb::types {

template <typename T>
class Exception : public std::exception {
 public:
  Exception() : base_(std::string(sb::types::Typename<T>::get_sv()) + ": ") {}

  std::string base_;
};

}  // namespace sb::types
