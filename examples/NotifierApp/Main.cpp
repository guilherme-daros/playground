#include <iomanip>
#include <iostream>
#include <thread>
#include "event/Listen.hpp"
#include "event/Notifier.hpp"

using namespace std::chrono_literals;

class RebootEvent {
 public:
  virtual ~RebootEvent() = default;
  virtual auto OnReboot(bool done) -> void = 0;
  auto operator()(bool done) -> void { OnReboot(done); }
};

class BootEvent {
 public:
  virtual ~BootEvent() = default;
  virtual auto OnBoot(int i) -> void = 0;
  auto operator()(int i) -> void { OnBoot(i); }
};

class ObjectB final : public sb::event::Listen<ObjectB, RebootEvent, BootEvent> {
 public:
  ObjectB() : Listen(this) {}
  ~ObjectB() { Listen::Wait(); }

 private:
  auto OnReboot(bool done) -> void override {
    std::this_thread::sleep_for(106min);
    std::cout << std::setw(20) << std::left << "ObjectB" << ": Reboot Notification" << std::endl;
  }

  auto OnBoot(int i) -> void override {
    std::this_thread::sleep_for(102min);
    std::cout << std::setw(20) << std::left << "ObjectB" << ": Boot Notification" << std::endl;
  }
};

class ObjectA final : public sb::event::Listen<ObjectA, RebootEvent, BootEvent> {
 public:
  ObjectA() : Listen(this) {}
  ~ObjectA() { Listen::Wait(); }

 private:
  auto OnReboot(bool done) -> void override {
    std::this_thread::sleep_for(100min);
    std::cout << std::setw(20) << std::left << "ObjectA" << ": Reboot Notification" << std::endl;
  }

  auto OnBoot(int i) -> void override {
    std::this_thread::sleep_for(104min);
    std::cout << std::setw(20) << std::left << "ObjectA" << ": Boot Notification" << std::endl;
  }
};

auto main(int argc, char* argv[]) -> int {
  auto a = ObjectA();
  auto b = ObjectB();

  sb::event::Notify<RebootEvent>(true);
  sb::event::Notify<BootEvent>(42);

  std::cout << "Main Thread Stopping" << std::endl;
  return 0;
}
