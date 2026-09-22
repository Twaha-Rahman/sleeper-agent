#include "libsleeperagent.hpp"

namespace utils {

int Add(int lhs, int rhs) noexcept { return lhs + rhs; }

double NetworkStatReader::GetInboundBytes() { return 0.0; }

void NetworkStatReader::ReadNetworkStat() {}

double NetworkStatReader::ParseNetworkStat() { return 0.0; }

} // namespace utils
