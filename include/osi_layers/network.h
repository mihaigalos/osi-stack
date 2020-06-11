#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "datalink.h"

template <typename DatalinkLayer = Datalink<Physical, CRC>>
class Network
{
public:
    Network(uint8_t own_id, DatalinkLayer &&datalink) : own_id_{own_id},
                                                        datalink_{std::forward<DatalinkLayer>(datalink)} {}
    Network(Network &&other) : own_id_{other.own_id_}, datalink_{std::forward<DatalinkLayer>(other.datalink_)} {}

    CommunicationStatus Transmit(const uint8_t to, Payload &payload) const
    {
        payload.data[payload.size++] = own_id_;
        payload.data[payload.size++] = to;
        log_dump_network(payload, "Network :: Transmit");
        return datalink_.Transmit(payload);
    }
    Payload Receive(uint8_t from) const
    {

        Payload received = datalink_.Receive();
        bool is_expected_message{
            received.data[received.size - kPosFromEndDestinationIdInPayload] == own_id_ &&
            received.data[received.size - kPosFromEndSourceIdInPayload] == from};

        log("Network :: ReceivedFrom received.data [from]: " + std::to_string(received.data[received.size - kPosFromEndDestinationIdInPayload]));
        log("Network :: ReceivedFrom received.data [to]: " + std::to_string(received.data[received.size - kPosFromEndSourceIdInPayload]));
        log("Network :: ReceivedFrom from: " + std::to_string(from));
        log("Network :: ReceivedFrom own_id_: " + std::to_string(own_id_));

        if (is_expected_message)
        {
            log_dump_network(received, "Network :: Receive");
        }

        return is_expected_message ? received : Payload{};
    }

#ifdef TESTING
    Network()
    {
    }
#endif

    virtual ~Network() = default;
    Network(const Network &other) = delete;

    Network &operator=(const Network &other) = delete;
    Network &operator=(Network &&other) = delete;

private:
    uint8_t own_id_{};
    DatalinkLayer datalink_;
};