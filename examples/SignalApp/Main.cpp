#include <ostream>
#include "event/Listen.hpp"
#include "logger/Logger.hpp"
#include "system/SignalManager.hpp"

namespace {
using AppLogger = sb::logger::Logger<"SignalApp">;
}  // namespace

class ObjectA final : public sb::event::Listen<sb::system::Signal::SIGINT> {
 public:
  ObjectA() : Listen(this) {}
  ~ObjectA() { Listen::Wait(); }

  void onSIGINT() override { LogA::Info() << "Received " << static_cast<int>(sb::system::Signal::SIGINT::I); }

 private:
  using LogA = sb::logger::Logger<"LogA">;
};

int main() {
  AppLogger::logging_level = sb::logger::Level::Debug;
  auto res = sb::system::SignalManager::enable(sb::system::Signal::Enum::SIGINT);
  if (!res) {
    AppLogger::Error() << "Failed to enable signal: " << res.error();
    return 1;
  }

  AppLogger::Debug() << "Creating ObjectA";
  auto a = ObjectA();

  AppLogger::Debug() << "Halting MainThread";
  std::cin.get();
  return 0;
}
