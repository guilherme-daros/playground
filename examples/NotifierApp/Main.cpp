#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "event/Listen.hpp"
#include "event/Notifier.hpp"
#include "logger/Logger.hpp"
#include "types/Exception.hpp"

using namespace std::chrono_literals;

class ExceptionA : public sb::types::Exception<ExceptionA> {
 public:
  ExceptionA(const std::string& msg) : message_(base_ + msg) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
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

    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

    if ((rand() % 10) > 5) {
      throw ExceptionA("Luck is not on our side");
    }

    LogA::Info() << "Received EventA ";
  }
};

auto main() -> int {
  auto a = ObjectA();

  for (auto i = 0; i < 10; i++) {
    sb::event::Notify<EventA>(true);
    std::this_thread::sleep_for(1ms);
  }

  std::cout << "Main Thread Stopping";
  return 0;
}
