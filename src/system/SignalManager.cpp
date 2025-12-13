#include "system/SignalManager.hpp"

#include <unistd.h>
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

auto SignalManager::enable(Signal::Enum s) -> void {
  struct sigaction sa = {};
  sa.sa_handler = posix_signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  enabled_signals_.emplace_back(static_cast<Signal::Enum>(s));

  for (auto& signal : enabled_signals_) {
    sigaction(static_cast<int>(signal), &sa, NULL);
  }
}

auto SignalManager::enableAll() -> void {
  struct sigaction sa = {};
  sa.sa_handler = posix_signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  for (auto i = 0; i <= 31; i++) {
    sigaction(i, &sa, NULL);
    enabled_signals_.emplace_back(static_cast<Signal::Enum>(i));
  }
}

SignalManager::~SignalManager() {
  close(pipe_fds_[1]);

  if (bridge_thread_.joinable()) {
    bridge_thread_.join();
  }

  close(pipe_fds_[0]);
}
}  // namespace sb::system
