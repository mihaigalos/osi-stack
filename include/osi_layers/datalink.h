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
    NoAcknowledgeRequired = 0x02,
    CRCMismatch = 0x03,
    Acknowledge = 0x06,
    NegativeAcknowledge = 0x15,
};

template <typename PhysicalLayer = Physical, typename CRCFunctions = CRC>
class Datalink
{
public:
    Datalink(PhysicalLayer &&physical) : physical_{std::forward<PhysicalLayer>(physical)} {}
    Datalink(Datalink &&other) : physical_{std::forward<PhysicalLayer>(other.physical_)}, retransmit_count_{other.retransmit_count_} {}
    CommunicationStatus Transmit(const Payload &payload) const;
    Payload Receive() const;

    virtual ~Datalink() = default;
    Datalink &operator=(const Datalink &other) = delete;
    Datalink &operator=(Datalink &&other) = delete;

private:
    PhysicalLayer physical_;
    CRCFunctions crc_;
    uint8_t retransmit_count_{kMaxRetransmitCount};

    Payload GetTransmitResponse() const;
};