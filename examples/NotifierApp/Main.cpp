#include <iomanip>
#include <iostream>
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

class Manager final : public sb::event::Listen<Manager, RebootEvent, BootEvent> {
 public:
  Manager() : Listen(this) {}
  ~Manager() { Listen::Wait(); }

 private:
  auto OnReboot(bool done) -> void override {
    std::this_thread::sleep_for(100ms);
    std::cout << std::setw(20) << std::left << "Manager" << ": Reboot Notification" << std::endl;
  }

  auto OnBoot(int i) -> void override {
    std::this_thread::sleep_for(102ms);
    std::cout << std::setw(20) << std::left << "Manager" << ": Boot Notification" << std::endl;
  }
};

class Coordinator final : public sb::event::Listen<Coordinator, RebootEvent, BootEvent> {
 public:
  Coordinator() : Listen(this) {}
  ~Coordinator() { Listen::Wait(); }

 private:
  auto OnReboot(bool done) -> void override {
    std::this_thread::sleep_for(100ms);
    std::cout << std::setw(20) << std::left << "Coordinator" << ": Reboot Notification" << std::endl;
  }

  auto OnBoot(int i) -> void override {
    std::this_thread::sleep_for(102ms);
    std::cout << std::setw(20) << std::left << "Coordinator" << ": Boot Notification" << std::endl;
  }
};

auto main(int argc, char* argv[]) -> int {
  auto m = Manager();

  {
    auto c = Coordinator();
    sb::event::Notify<RebootEvent>(true);
  }

  sb::event::Notify<BootEvent>(42);

  std::cout << "Main Thread Stopping" << std::endl;
  return 0;
}
