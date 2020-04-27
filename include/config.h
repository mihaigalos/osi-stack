#pragma once

#include <cstdint>

using CRCChecksum = uint16_t;
constexpr uint8_t kCRCSize = sizeof(CRCChecksum);

const uint8_t kMaxRetransmitCount{2};