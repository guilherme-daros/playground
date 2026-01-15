#pragma once

#include <thread>

#include "logger/Logger.hpp"
#include "system/Signals.hpp"
#include "types/Result.hpp"
#include "types/Singleton.hpp"

namespace sb::system {

class SignalManager final : public sb::types::Singleton<SignalManager> {
 public:
  ~SignalManager();
  SignalManager();

  static auto enableAll() -> Result<void>;
  static auto enable(Signal::Enum s) -> Result<void>;

 private:
  auto bridge_thread() -> void {
    using BridgeThread = sb::logger::Logger<"BridgeThread">;
    BridgeThread::logging_level = sb::logger::Level::Debug;

    BridgeThread::Debug() << "Started.";
    int signum;

    while (read(pipe_fds_[0], &signum, sizeof(signum)) > 0) {
      auto sig = static_cast<Signal::Enum>(signum);

      sb::system::Notify(sig);
    }

    BridgeThread::Debug() << "Finished.";
  }
  static auto posix_signal_handler(int signum) -> void;

  std::thread bridge_thread_;
  static std::vector<Signal::Enum> enabled_signals_;
  int pipe_fds_[2];
};

}  // namespace sb::system
