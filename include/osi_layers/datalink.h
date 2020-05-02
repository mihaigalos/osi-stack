#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "crc.h"

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
    Datalink(TVoidUint8 on_transmit, TUint8Void on_receive) : io_{on_transmit, on_receive} {}
    CommunicationStatus TransmitWithAcknowledge(const Payload &payload, uint8_t retransmit_count = kMaxRetransmitCount) const;
    Payload ReceiveWithAcknowledge() const;

    virtual ~Datalink() = default;

    Datalink(Datalink &&other) = delete;
    Datalink(const Datalink &other) = delete;
    Datalink &operator=(const Datalink &other) = delete;
    Datalink &operator=(Datalink &&other) = delete;

private:
    PhysicalLayer io_;
    CRCFunctions crc_;
};