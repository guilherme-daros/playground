#include <iostream>
#include "types/EnumStr.hpp"

enum class MyEnum { Value1, Value2, Value3 };

int main() {
  constexpr auto name1 = sb::types::EnumStr<MyEnum::Value1>::get();
  constexpr auto name2 = sb::types::EnumStr<MyEnum::Value2>::get();
  constexpr auto name3 = sb::types::EnumStr<MyEnum::Value3>::get();

  std::cout << "Enum value 1: " << name1.data;
  std::cout << "Enum value 2: " << name2.data;
  std::cout << "Enum value 3: " << name3.data;

  constexpr auto full_name1 = sb::types::EnumStr<MyEnum::Value1>::get_full();
  constexpr auto full_name2 = sb::types::EnumStr<MyEnum::Value2>::get_full();
  constexpr auto full_name3 = sb::types::EnumStr<MyEnum::Value3>::get_full();

  std::cout << "Enum full name 1: " << full_name1.data;
  std::cout << "Enum full name 2: " << full_name2.data;
  std::cout << "Enum full name 3: " << full_name3.data;

  return 0;
}
