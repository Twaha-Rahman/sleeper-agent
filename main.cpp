#include "libseed.hpp"

#include <bits/types/sigset_t.h>
#include <chrono>
#include <sys/poll.h>
#include <thread>

#include <csignal>
#include <cstdio>
#include <poll.h>
#include <sys/signalfd.h>
#include <systemd/sd-journal.h>

int main() {
  // NOLINTNEXTLINE
  int _ = seed::Add(2, 3);

  // handle SIGINT so it isn't handled the default way
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &mask, nullptr) < 0) {
    perror("Error occured on call to `sigprocmask()`");
    return 1;
  }

  // create the file descriptor where signals will arrive on
  int sfd = signalfd(-1, &mask, 0);
  if (sfd < 0) {
    perror("Error occured on call to `signalfd()`");
    return 1;
  }

  sd_journal_send("MESSAGE=SleeperAgent service started...", "PRIORITY=%d", 6,
                  NULL);

  pollfd pfd{.fd = sfd, .events = POLLIN, .revents = 0};
  while (true) {
    int r = poll(&pfd, 1, -1);
    if (r < 0) {
      if (errno == EINTR) {
        continue;
      }

      perror("Error occured on call to `poll()`");
      break;
    }

    if (pfd.revents & POLLIN) {
      signalfd_siginfo si{}; // NOLINT(readability-identifier-length)
      if (read(sfd, &si, sizeof(si)) != sizeof(si)) {
        perror("Error occured on call to `read()`");
        break;
      }

      if (si.ssi_signo == SIGINT) {
        sd_journal_send(
            "MESSAGE=SleeperAgent received SIGINT; Shutting down gracefully...",
            "PRIORITY=%d", 5, "SIGNAL=SIGINT", "SIGNAL_NUM=%d",
            (int)si.ssi_signo, NULL);
        break;
      }

      if (si.ssi_signo == SIGTERM) {
        sd_journal_send("MESSAGE=SleeperAgent received SIGTERM; Shutting down "
                        "gracefully...",
                        "PRIORITY=%d", 5, "SIGNAL=SIGTERM", "SIGNAL_NUM=%d",
                        (int)si.ssi_signo, NULL);
        break;
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

  return 0;
}
