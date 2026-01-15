#include <iostream>
#include "types/Typename.hpp"

namespace my_namespace {
class MyClass {};
}  // namespace my_namespace

int main() {
  constexpr auto int_name = sb::types::Typename<int>::get();
  constexpr auto double_name = sb::types::Typename<double>::get();
  constexpr auto my_class_name = sb::types::Typename<my_namespace::MyClass>::get();

  std::cout << "Type name of int: " << int_name.data;
  std::cout << "Type name of double: " << double_name.data;
  std::cout << "Type name of MyClass: " << my_class_name.data;

  return 0;
}
