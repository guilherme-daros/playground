#pragma once

#include <chrono>
#include <concepts>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include "Meta.hpp"

namespace sb::logger::timing {

template <typename T>
concept IsTiming = requires(T t) {
  { t.get() } -> std::same_as<std::string_view>;
  { t.width() } -> std::same_as<uint8_t>;
};

class Uptime {
 public:
  using Default = Uptime;
  auto get() const -> std::string_view {
    static auto start = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto uptime_s = std::chrono::duration_cast<std::chrono::seconds>(now - start);
    auto uptime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start) % 1000;
    auto os = std::ostringstream();
    os << std::setw(seconds_width) << std::right << uptime_s.count() << "." << std::setw(milliseconds_width)
       << std::setfill('0') << (uptime_ms.count());

    static thread_local auto uptime = std::string(os.str());
    return uptime;
  }
  constexpr auto width() const -> uint8_t { return seconds_width + milliseconds_width; }

 private:
  static constexpr uint8_t seconds_width = 5;
  static constexpr uint8_t milliseconds_width = 3;
};

class Timestamp {
 public:
  auto get() const -> std::string_view {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    auto os = std::ostringstream();
    os << std::put_time(std::localtime(&time), "%H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms.count();

    static thread_local auto timestamp = std::string(os.str());
    return timestamp;
  }
  constexpr auto width() const -> uint8_t { return timestamp_width; }

 private:
  static constexpr uint8_t timestamp_width = 12;
};

constexpr auto is_timing = []<typename T>() { return IsTiming<T>; };

template <typename... Ts>
using Timing = meta::TypeFinder_t<Uptime, is_timing, Ts...>;

}  // namespace sb::logger::timing
