
#include "osi_layers/network.h"
#include "utilities.h"
#include "crc.h"

template <>
CommunicationStatus Network<>::TransmitTo(Payload &payload, uint8_t to) const
{
    CommunicationStatus result{CommunicationStatus::Unknown};

    if (payload.size + sizeof(to) + sizeof(own_id_) < kPayloadMaxSize)
    {
        payload.data[payload.size++] = to;
        payload.data[payload.size++] = own_id_;
        log_dump_payload(payload, "Network :: TransmitTo");
        return datalink_.TransmitWithAcknowledge(payload);
    }
    else
    {
        log("Size of payload > maximum allowed size");
        log_dump_payload(payload, "Network :: TransmitTo");
    }

    return result;
}

template <>
Payload Network<>::ReceiveFrom(uint8_t from) const
{
    Payload received;
    static_cast<void>(from);

    return received;
}
