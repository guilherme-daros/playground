#pragma once

#include <source_location>
#include <string_view>
#include <utility>
#include "StringLiteral.hpp"

namespace sb::types {

namespace detail {
template <size_t... Is>
consteval auto make_string_literal_from_sv(std::string_view sv, std::index_sequence<Is...>) {
  char buf[] = {sv[Is]..., '\0'};
  return StringLiteral(buf);
}
}  // namespace detail

template <auto EnumValue>
class EnumStr {
  static consteval auto get_sv() {
    std::string_view name{std::source_location::current().function_name()};

    auto pos = name.rfind("::");
    if (pos == std::string_view::npos) {
      // gcc
      pos = name.find("EnumValue = ");
      if (pos != std::string_view::npos) {
        pos += 12;
      }
    } else {
      // clang
      pos += 2;
    }

    auto end = name.find_first_of("])", pos);

    return name.substr(pos, end - pos);
  }

  static consteval auto get_full_sv() {
    std::string_view name{std::source_location::current().function_name()};
    auto pos = name.find(" = ");
    if (pos == std::string_view::npos) {
      return std::string_view{};
    }
    pos += 3;
    auto end = name.find_last_of("]");
    return name.substr(pos, end - pos);
  }

 public:
  static consteval auto get() {
    constexpr auto sv = get_sv();
    if constexpr (sv.empty()) {
      return StringLiteral("");
    } else {
      return detail::make_string_literal_from_sv(sv, std::make_index_sequence<sv.size()>{});
    }
  }

  static consteval auto get_full() {
    constexpr auto sv = get_full_sv();
    if constexpr (sv.empty()) {
      return StringLiteral("");
    } else {
      return detail::make_string_literal_from_sv(sv, std::make_index_sequence<sv.size()>{});
    }
  }
};
}  // namespace sb::types
