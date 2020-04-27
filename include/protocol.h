#pragma once

#include <cstdint>
#include "transceiver.h"
#include "config.h"

enum class CommunicationStatus : uint8_t
{
    Unknown = 0x00,
    Error = 0x01,
    Acknowledge = 0x06,
    NegativeAcknowledge = 0x15
};

template <typename T = Transceiver>
class UartHandshake
{
public:
    UartHandshake(TVoidUint8 on_transmit, TUint8Void on_receive) : io_{on_transmit, on_receive} {}
    CommunicationStatus TransmitWithAcknowledge(const Payload &payload, uint8_t retransmit_count = kMaxRetransmitCount) const;
    Payload ReceiveWithAcknowledge(uint8_t expected_count = 1 + kCRCSize) const;

private:
    T io_;
};