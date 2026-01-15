#include "system/SignalManager.hpp"

#include <unistd.h>
#include <algorithm>
#include <csignal>

namespace sb::system {

std::vector<Signal::Enum> SignalManager::enabled_signals_;

void SignalManager::posix_signal_handler(int signum) {
  auto&& instance = Instance();
  write(instance.pipe_fds_[1], &signum, sizeof(signum));
}

SignalManager::SignalManager() {
  if (pipe(pipe_fds_) == -1) {
    throw std::runtime_error("Failed to create pipe for SignalManager.");
  }

  bridge_thread_ = std::thread(&SignalManager::bridge_thread, this);
}

auto SignalManager::enable(Signal::Enum s) -> Result<void> {
  if (std::find(enabled_signals_.begin(), enabled_signals_.end(), s) != enabled_signals_.end()) {
    return {};
  }

  struct sigaction sa = {};
  sa.sa_handler = posix_signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(static_cast<int>(s), &sa, NULL) != 0) {
    return std::unexpected("Failed to register signal handler for signal " + std::to_string(static_cast<int>(s)));
  }

  enabled_signals_.emplace_back(s);
  return {};
}

auto SignalManager::enableAll() -> Result<void> {
  struct sigaction sa = {};
  sa.sa_handler = posix_signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  for (auto i = 1; i <= 31; i++) {
    if (sigaction(i, &sa, NULL) != 0) {
      return std::unexpected("Failed to register signal handler for signal " + std::to_string(i));
    }
    if (std::find(enabled_signals_.begin(), enabled_signals_.end(), static_cast<Signal::Enum>(i)) ==
        enabled_signals_.end()) {
      enabled_signals_.emplace_back(static_cast<Signal::Enum>(i));
    }
  }
  return {};
}

SignalManager::~SignalManager() {
  close(pipe_fds_[1]);

  if (bridge_thread_.joinable()) {
    bridge_thread_.join();
  }

  close(pipe_fds_[0]);
}
}  // namespace sb::system
