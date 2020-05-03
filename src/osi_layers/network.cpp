
#include "osi_layers/network.h"
#include "utilities.h"

template <>
CommunicationStatus Network<>::Transmit(uint8_t to, Payload &payload) const
{
    payload.data[payload.size++] = to;
    payload.data[payload.size++] = own_id_;
    log_dump_payload(payload, "Network :: Transmit");
    return datalink_.TransmitWithAcknowledge(payload);
}

template <>
Payload Network<>::ReceiveFrom(uint8_t from) const
{
    Payload received = datalink_.ReceiveWithAcknowledge();
    bool is_expected_message{received.data[kPosDestinationIdInPayload] == own_id_ && received.data[kPosSourceIdInPayload] == from};
    log("Network :: ReceivedFrom received.data[kPosDestinationIdInPayload]: " + std::to_string(received.data[kPosDestinationIdInPayload]));
    log("Network :: ReceivedFrom received.data[kPosSourceIdInPayload]: " + std::to_string(received.data[kPosSourceIdInPayload]));
    log("Network :: ReceivedFrom own_id_: " + std::to_string(own_id_));
    log("Network :: ReceivedFrom from: " + std::to_string(from));

    if (is_expected_message)
    {
        log_dump_payload(received, "Network :: ReceiveFrom");
    }

    return is_expected_message ? received : Payload{};
}
