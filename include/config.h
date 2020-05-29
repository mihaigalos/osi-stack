#pragma once

#include <cstdint>

#include <containers/static_string.h>

using TCRCChecksum = uint16_t;

constexpr uint8_t kCRCSize{sizeof(TCRCChecksum)};
constexpr uint8_t kMaxRetransmitCount{2};
constexpr uint8_t kPayloadMaxSize{16};

constexpr uint8_t kMaximumSegments{10};
using TSegment = uint16_t;

constexpr uint8_t kPosFromEndDestinationIdInPayload{3};
constexpr uint8_t kPosFromEndSourceIdInPayload{4};

constexpr uint8_t kSizeOfToField{1};
constexpr uint8_t kSizeOfFromField{1};

constexpr uint8_t kTransportPayloadSize{kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - kCRCSize - sizeof(TSegment)};

constexpr uint8_t kStaticStringSize{kMaximumSegments * (kPayloadMaxSize - kTransportPayloadSize)};
//TODO: adjust this containers::static_string size depending on how much of the payload is actually available once the full stack is implemented (after subtracting the metadata).
using TString = containers::static_string<kStaticStringSize>;

class Payload;
using TMap = containers::static_map<uint8_t, Payload, kMaximumSegments * kTransportPayloadSize>;