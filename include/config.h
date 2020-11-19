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

using TMapClientsCookies = containers::static_map<uint8_t, uint16_t, kMaximumPorts>;

class Payload;
using TMapSequencePayload = containers::static_map<uint8_t, Payload, kMaximumSegments * kTransportPayloadSize>;
using TMapPortSequencePayload = containers::static_map<uint8_t, TMapSequencePayload, kMaximumPorts>;

constexpr uint16_t kCookieBaseValue = 0xBEEF;
constexpr char kCookieBaseValueStringified[]{static_cast<char>((kCookieBaseValue >> 8) & 0xFF), static_cast<char>(kCookieBaseValue & 0x00FF), '\0'};

constexpr uint8_t kEncryptionKeySize{16};
constexpr uint8_t kEncryptionKeyMaxIndex{kEncryptionKeySize - 1};
constexpr uint8_t kEncryptionKey[kEncryptionKeySize] = {0x45, 0x74, 0x32, 0x11, 0x98, 0x94, 0xAB, 0xCF, 0x90, 0xAE, 0xBA, 0xDC, 0x06, 0x16, 0x81, 0x95};
constexpr uint8_t kEncryptionDelta{0x39};

#ifdef TESTING
#define virtual__ virtual
extern void onPreTransmitData();
#else
#define virtual__
#define onPreTransmitData()
#endif
