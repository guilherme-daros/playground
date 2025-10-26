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

  void onSIGINT() override { LogA::Info() << "Received " << static_cast<int>(I) << std::endl; }

 private:
  using LogA = sb::logger::Logger<"LogA">;
};

int main() {
  AppLogger::logging_level = sb::logger::Level::Debug;
  sb::system::SignalManager::enable(sb::system::Signal::Enum::SIGINT);

  AppLogger::Debug() << "Creating ObjectA" << std::endl;
  auto a = ObjectA();

  AppLogger::Debug() << "Halting MainThread" << std::endl;
  std::cin.get();
  return 0;
}
