#pragma once

#include <cstdint>

#include <containers/static_string.h>

using CRCChecksum = uint16_t;

constexpr uint8_t kCRCSize{sizeof(CRCChecksum)};
constexpr uint8_t kMaxRetransmitCount{2};
constexpr uint8_t kPayloadMaxSize{16};

constexpr uint8_t kSizeOfSegment{50};

using TString = containers::static_string<kSizeOfSegment>;
using TMap = containers::static_map<uint8_t, uint8_t, kSizeOfSegment>;