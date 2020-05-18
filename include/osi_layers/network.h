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

    CommunicationStatus Transmit(const uint8_t to, Payload &payload) const;
    Payload Receive(uint8_t from) const;

    virtual ~Network() = default;
    Network(const Network &other) = delete;

    Network &operator=(const Network &other) = delete;
    Network &operator=(Network &&other) = delete;

private:
    uint8_t own_id_{};
    DatalinkLayer datalink_;
};