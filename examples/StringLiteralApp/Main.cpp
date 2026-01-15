#include <array>
#include <iostream>
#include "types/StringLiteral.hpp"

template <sb::types::StringLiteral db_name>
class MyClass {
 public:
  constexpr static std::array<char, db_name.size() + 1> db_name_ = [] {
    std::array<char, db_name.size() + 1> arr{};
    std::copy_n(db_name.data, db_name.size() + 1, arr.begin());
    return arr;
  }();
};

int main() { std::cout << MyClass<"Help">::db_name_.data(); }
