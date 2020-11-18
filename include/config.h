#pragma once

#include <cstdint>

#include <containers/static_string.h>

using TCRCChecksum = uint16_t;

constexpr uint8_t kCRCSize{sizeof(TCRCChecksum)};
constexpr uint8_t kMaxRetransmitCount{2};
constexpr uint8_t kPayloadMaxSize{20};

constexpr uint8_t kMaximumSegments{5};
constexpr uint8_t kMaximumPorts{5};
using TSegment = uint16_t;

constexpr uint8_t kPosFromEndDestinationIdInPayload{3};
constexpr uint8_t kPosFromEndSourceIdInPayload{4};

constexpr uint8_t kSizeOfSizeField{1};
constexpr uint8_t kSizeOfToField{1};
constexpr uint8_t kSizeOfFromField{1};
constexpr uint8_t kSizeOfPort{1};
constexpr uint8_t kSizeOfCookie{2};

constexpr uint8_t kTransportPayloadSize{kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - sizeof(TSegment) - kSizeOfPort - kCRCSize};

constexpr uint8_t kStaticStringSize{kMaximumSegments * (kPayloadMaxSize - kTransportPayloadSize)};
//TODO: adjust this containers::static_string size depending on how much of the payload is actually available once the full stack is implemented (after subtracting the metadata).
using TString = containers::static_string<kStaticStringSize>;

class Payload;
using TMapSequencePayload = containers::static_map<uint8_t, Payload, kMaximumSegments * kTransportPayloadSize>;
using TMapPortSequencePayload = containers::static_map<uint8_t, TMapSequencePayload, kMaximumPorts>;

constexpr char kCookieBaseValue[]{"\xBE\xEF"};

#ifdef TESTING
#define virtual__ virtual
extern void onPreTransmitData();
#else
#define virtual__
#define onPreTransmitData()
#endif