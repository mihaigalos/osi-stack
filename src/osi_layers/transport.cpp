
#include "osi_layers/transport.h"
#include "utilities.h"
#include "crc.h"

template <>
CommunicationStatus Transport<>::TransmitTo(const Payload &payload, uint8_t to) const
{
    CommunicationStatus result{CommunicationStatus::Unknown};

    static_cast<void>(payload);
    static_cast<void>(to);

    return result;
}

template <>
Payload Transport<>::ReceiveFrom(uint8_t from) const
{
    Payload received;
    static_cast<void>(from);

    return received;
}
