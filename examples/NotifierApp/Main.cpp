#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include "event/Listen.hpp"
#include "event/Notifier.hpp"

using namespace std::chrono_literals;

class EventA {
 public:
  virtual ~EventA() = default;
  virtual auto OnA(bool done) -> void = 0;
  auto operator()(bool done) -> void { OnA(done); }
};

class EventB {
 public:
  virtual ~EventB() = default;
  virtual auto OnB(int i) -> void = 0;
  auto operator()(int i) -> void { OnB(i); }
};

class ObjectA final : public sb::event::Listen<EventA> {
 public:
  ObjectA() : Listen(this) {}
  ~ObjectA() { Listen::Wait(); }

 private:
  auto OnA(bool done) -> void override {
    (void)done;
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 100));
    std::cout << std::setw(20) << std::left << "ObjectA" << ": Received EventA" << std::endl;
  }
};

class ObjectB final : public sb::event::Listen<EventA, EventB> {
 public:
  ObjectB() : Listen(this) {}
  ~ObjectB() { Listen::Wait(); }

 private:
  auto OnA(bool done) -> void override {
    (void)done;
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 100));
    std::cout << std::setw(20) << std::left << "ObjectB" << ": Received EventA" << std::endl;
  }

  auto OnB(int i) -> void override {
    (void)i;
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 100));
    std::cout << std::setw(20) << std::left << "ObjectB" << ": Received EventB" << std::endl;
  }
};

class ObjectC final : public sb::event::Listen<EventB> {
 public:
  ObjectC() : Listen(this) {}

 private:
  auto OnB(int i) -> void override {
    (void)i;
    std::cout << std::setw(20) << std::left << "ObjectC" << ": Received EventB" << std::endl;
  }
};

auto main() -> int {
  auto a = ObjectA();
  auto b = ObjectB();
  auto c = ObjectC();

  for (auto i = 0; i < 10; i++) {
    sb::event::Notify<EventA>(true);
    sb::event::Notify<EventB>(42);
    sb::event::Notify<EventA>(false);
    std::this_thread::sleep_for(50ms);
  }

  std::cout << "Main Thread Stopping" << std::endl;
  return 0;
}
