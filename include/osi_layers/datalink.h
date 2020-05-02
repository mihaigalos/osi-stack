#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "crc.h"
#include <utility>

enum class CommunicationStatus : uint8_t
{
    Unknown = 0x00,
    Error = 0x01,
    Acknowledge = 0x06,
    NegativeAcknowledge = 0x15
};

template <typename PhysicalLayer = Physical, typename CRCFunctions = CRC>
class Datalink
{
public:
    Datalink(PhysicalLayer &&physical) : physical_{std::forward<PhysicalLayer>(physical)} {}
    Datalink(Datalink &&other) : physical_{std::forward<PhysicalLayer>(other.physical_)} {}
    CommunicationStatus TransmitWithAcknowledge(const Payload &payload, uint8_t retransmit_count = kMaxRetransmitCount) const;
    Payload ReceiveWithAcknowledge() const;

    virtual ~Datalink() = default;
    Datalink &operator=(const Datalink &other) = delete;
    Datalink &operator=(Datalink &&other) = delete;

private:
    PhysicalLayer physical_;
    CRCFunctions crc_;
};