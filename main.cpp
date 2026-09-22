#include "libsleeperagent.hpp"

#include <bits/types/sigset_t.h>
#include <chrono>
#include <sys/poll.h>
#include <thread>

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <poll.h>
#include <sys/signalfd.h>
#include <systemd/sd-journal.h>
#include <unistd.h>

int main() {
  // NOLINTNEXTLINE
  int _ = utils::Add(2, 3);

  // handle SIGINT so it isn't handled the default way
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &mask, nullptr) < 0) {
    perror("Error occurred on call to `sigprocmask()`");
    sd_journal_send(
        "MESSAGE=[[SleeperAgent]] ERROR: Failed to apply signal mask",
        "PRIORITY=%d", 3, NULL);
    return 1;
  }

  // create the file descriptor where signals will arrive on
  int sfd = signalfd(-1, &mask, 0);
  if (sfd < 0) {
    perror("Error occurred on call to `signalfd()`");
    sd_journal_send("MESSAGE=[[SleeperAgent]] ERROR: Failed to create "
                    "signal file descriptor (sfd)",
                    "PRIORITY=%d", 3, NULL);
    return 1;
  }

  sd_journal_send("MESSAGE=[[SleeperAgent]] Daemon started watching",
                  "PRIORITY=%d", 6, NULL);

  pollfd pfd{.fd = sfd, .events = POLLIN, .revents = 0};
  bool has_poll_failed = false;
  bool has_read_failed = false;
  while (true) {
    int r = poll(&pfd, 1, -1);
    if (r < 0) {
      if (errno == EINTR) {
        continue;
      }

      has_poll_failed = true;
      perror("Error occurred on call to `poll()`");
      sd_journal_send("MESSAGE=[[SleeperAgent]] ERROR: Failed to poll "
                      "signal file descriptor (sfd)",
                      "PRIORITY=%d", 3, NULL);
      break;
    }

    // if (pfd.revents & POLLIN) {
    if ((pfd.revents & POLLIN) == 0) {
      continue;
    }

    signalfd_siginfo si{}; // NOLINT(readability-identifier-length)
    if (read(sfd, &si, sizeof(si)) != sizeof(si)) {
      has_read_failed = true;
      perror("Error occurred on call to `read()`");
      sd_journal_send("MESSAGE=[[SleeperAgent]] ERROR: Failed to read from "
                      "signal file descriptor (sfd)",
                      "PRIORITY=%d", 3, NULL);
      break;
    }

    if (si.ssi_signo == SIGINT) {
      sd_journal_send("MESSAGE=[[SleeperAgent]] received SIGINT; Shutting down "
                      "gracefully...",
                      "PRIORITY=%d", 5, "SIGNAL=SIGINT", "SIGNAL_NUM=%d",
                      (int)si.ssi_signo, NULL);
      break;
    }
    if (si.ssi_signo == SIGTERM) {
      sd_journal_send(
          "MESSAGE=[[SleeperAgent]] received SIGTERM; Shutting down "
          "gracefully...",
          "PRIORITY=%d", 5, "SIGNAL=SIGTERM", "SIGNAL_NUM=%d",
          (int)si.ssi_signo, NULL);
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

  if (close(sfd)) {
    sd_journal_send("MESSAGE=[[SleeperAgent]] WARN: Failed to close signal "
                    "file descriptor (sfd)",
                    "PRIORITY=%d", 4, "ERR_NO=%d", errno, NULL);
  }

  if (has_poll_failed) {
    return 1;
  }
  if (has_read_failed) {
    return 1;
  }

  sd_journal_send("MESSAGE=[[SleeperAgent]] Daemon exited successfully",
                  "PRIORITY=%d", 6, NULL);

  return 0;
}
