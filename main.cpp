#include "libseed.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <print>
#include <thread>

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::atomic<bool> g_running{true};

void HandleSignal(int /*signal*/) { g_running.store(false); }
} // namespace

int main() { // NOLINT(bugprone-exception-escape)
  if (std::signal(SIGINT, HandleSignal) == SIG_ERR) {
    std::perror("Failed to install SIGINT handler");
    return 1;
  }
  if (std::signal(SIGTERM, HandleSignal) == SIG_ERR) {
    std::perror("Failed to install SIGTERM handler");
    return 1;
  }

  // Dummy library call to prove the linkage. Replace with real logic.
  std::println("daemon starting (2 + 3 = {})", seed::Add(2, 3));

  while (g_running.load()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::println("daemon shutting down");
  return 0;
}
