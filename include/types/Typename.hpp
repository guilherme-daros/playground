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

template <typename T>
class Typename {
 public:
  static consteval auto get() {
    constexpr auto type_sv = []() {
      std::string_view name{std::source_location::current().function_name()};

      auto pos = name.find("[with T = ");  // GCC
      if (pos != std::string_view::npos) {
        pos += 11;
        auto end = name.find(']', pos);
        return name.substr(pos, end - pos);
      }

      pos = name.find("[T = ");  // Clang
      if (pos != std::string_view::npos) {
        pos += 5;
        auto end = name.find(']', pos);
        return name.substr(pos, end - pos);
      }

      pos = name.find('<');
      if (pos == std::string_view::npos) return std::string_view{};
      auto end = name.find('>', pos);
      if (end == std::string_view::npos) return std::string_view{};

      return name.substr(pos + 1, end - pos - 1);
    }();

    if constexpr (type_sv.empty()) {
      return StringLiteral("");
    } else {
      return detail::make_string_literal_from_sv(type_sv, std::make_index_sequence<type_sv.size()>{});
    }
  }
  static consteval auto get_sv() {
    constexpr auto type_sv = []() {
      std::string_view name{std::source_location::current().function_name()};

      auto pos = name.find("[with T = ");  // GCC
      if (pos != std::string_view::npos) {
        pos += 11;
        auto end = name.find(']', pos);
        return name.substr(pos, end - pos);
      }

      pos = name.find("[T = ");  // Clang
      if (pos != std::string_view::npos) {
        pos += 5;
        auto end = name.find(']', pos);
        return name.substr(pos, end - pos);
      }

      pos = name.find('<');
      if (pos == std::string_view::npos) return std::string_view{};
      auto end = name.find('>', pos);
      if (end == std::string_view::npos) return std::string_view{};

      return name.substr(pos + 1, end - pos - 1);
    }();

    return type_sv;
  }
};
}  // namespace sb::types
