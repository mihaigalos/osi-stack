
#include "osi_layers/network.h"
#include "utilities.h"

template <>
CommunicationStatus Network<>::Transmit(const uint8_t to, Payload &payload) const
{
    payload.data[payload.size++] = own_id_;
    payload.data[payload.size++] = to;
    log_dump_payload(payload, "Network :: Transmit");
    return datalink_.Transmit(payload);
}

template <>
Payload Network<>::Receive(const uint8_t from) const
{
    Payload received = datalink_.Receive();
    bool is_expected_message{
        received.data[received.size - kPosFromEndDestinationIdInPayload] == own_id_ &&
        received.data[received.size - kPosFromEndSourceIdInPayload] == from};

    log("Network :: ReceivedFrom received.data[received.size - kPosFromEndDestinationIdInPayload]: " + std::to_string(received.data[received.size - kPosFromEndDestinationIdInPayload]));
    log("Network :: ReceivedFrom received.data[received.size - kPosFromEndSourceIdInPayload]: " + std::to_string(received.data[received.size - kPosFromEndSourceIdInPayload]));
    log("Network :: ReceivedFrom own_id_: " + std::to_string(own_id_));
    log("Network :: ReceivedFrom from: " + std::to_string(from));

    if (is_expected_message)
    {
        log_dump_payload(received, "Network :: Receive");
    }

    return is_expected_message ? received : Payload{};
}
