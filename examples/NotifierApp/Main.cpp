#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <thread>

#include "event/Listen.hpp"
#include "event/Notifier.hpp"
#include "logger/Logger.hpp"

using namespace std::chrono_literals;

class ExceptionA : public std::exception {
 private:
  std::string message;  // To store the custom error message
 public:
  ExceptionA(const std::string& msg) : message(msg) {}

  // Override the what() method to return the custom message
  const char* what() const noexcept { return message.c_str(); }
};

class EventA {
 public:
  virtual ~EventA() = default;
  virtual auto OnA(bool done) -> void = 0;
  auto operator()(bool done) -> void { OnA(done); }
};

class ObjectA final : public sb::event::Listen<EventA> {
 public:
  ObjectA() : Listen(this) {}
  ~ObjectA() { Listen::Wait(); }

 private:
  using LogA = sb::logger::Logger<"LogA">;

  auto OnA(bool done) -> void override {
    (void)done;

    std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));

    if ((rand() % 10) > 5) {
      throw ExceptionA("Luck is not on our side");
    }

    LogA::Info() << "Received EventA " << std::endl;
  }
};

auto main() -> int {
  auto a = ObjectA();

  for (auto i = 0; i < 100; i++) {
    sb::event::Notify<EventA>(true);
    std::this_thread::sleep_for(50ms);
  }

  std::cout << "Main Thread Stopping" << std::endl;
  return 0;
}
