
#include "osi_layers/network.h"
#include "utilities.h"
#include "crc.h"

template <>
CommunicationStatus Network<>::TransmitTo(const Payload &payload, uint8_t to) const
{
    CommunicationStatus result{CommunicationStatus::Unknown};

    static_cast<void>(payload);
    static_cast<void>(to);

    return result;
}

template <>
Payload Network<>::ReceiveFrom(uint8_t from) const
{
    Payload received;
    static_cast<void>(from);

    return received;
}
