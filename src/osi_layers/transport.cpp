#include "osi_layers/transport.h"

template <>
CommunicationStatus Transport<>::TransmitWithSplit(uint8_t to, uint8_t *data, uint32_t size) const
{
    auto result{CommunicationStatus::Unknown};
    Payload payload{};

    static_cast<void>(data);
    for (uint32_t i; i < size; ++i)
    {
        network_.Transmit(to, payload);
    }

    return result;
}

template <>
Payload Transport<>::ReceiveWithSplit(uint8_t from) const
{
    static_cast<void>(from);
    return {};
}
