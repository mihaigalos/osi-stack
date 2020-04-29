#pragma once

#include <cstdint>

using CRCChecksum = uint16_t;

constexpr uint8_t kCRCSize{sizeof(CRCChecksum)};
constexpr uint8_t kMaxRetransmitCount{2};
constexpr uint8_t kPayloadMaxSize{16};