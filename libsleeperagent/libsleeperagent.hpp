#pragma once

#include <string>

namespace utils {

// Example function. Replace with your library API.
[[nodiscard]] int Add(int lhs, int rhs) noexcept;

class NetworkStatReader {
public:
  NetworkStatReader() : m_read_path("/proc/net/dev") {}

  // NOLINTNEXTLINE(readability-identifier-length)
  NetworkStatReader(char *p) : m_read_path(p) {}

  double GetInboundBytes();

private:
  std::string m_read_path;

  void ReadNetworkStat();
  double ParseNetworkStat();
};

} // namespace utils
