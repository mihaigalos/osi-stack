#pragma once

#include <cstdint>
#include "uartio.h"

enum class CommunicationStatus : uint8_t
{
    Unknown = 0x00,
    Error = 0x01,
    Acknowledge = 0x06,
    NegativeAcknowledge = 0x15
};

template <typename T = UartIO>
class UartHandshake
{
public:
    UartHandshake(TVoidUint8 on_transmit, TUint8Void on_receive) : io_{on_transmit, on_receive} {}
    CommunicationStatus TransmitWithAcknowledge(const Payload &payload);

private:
    T io_;
};