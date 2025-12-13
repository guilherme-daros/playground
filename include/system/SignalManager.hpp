#pragma once

#include <thread>

#include "event/Notifier.hpp"
#include "logger/Logger.hpp"
#include "system/Signals.hpp"
#include "types/Singleton.hpp"

namespace sb::system {

class SignalManager final : public sb::types::Singleton<SignalManager> {
 public:
  ~SignalManager();
  SignalManager();

  static void enableAll();
  static void enable(Signal::Enum s);

 private:
  void bridge_thread() {
    using BridgeThread = sb::logger::Logger<"BridgeThread">;
    BridgeThread::logging_level = sb::logger::Level::Debug;

    BridgeThread::Debug() << "Started." << std::endl;
    int signum;

    while (read(pipe_fds_[0], &signum, sizeof(signum)) > 0) {
      auto sig = static_cast<Signal::Enum>(signum);
      switch (sig) {
        case Signal::Enum::SIGINT:
          sb::event::Notify<Signal::SIGINT>();
          break;
        default:
          break;
      }
    }

    BridgeThread::Debug() << "Finished." << std::endl;
  }
  static void posix_signal_handler(int signum);

  std::thread bridge_thread_;
  static std::vector<Signal::Enum> enabled_signals_;
  int pipe_fds_[2];
};

}  // namespace sb::system
