#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "datalink.h"

constexpr uint8_t kPosDestinationIdInPayload{4};
constexpr uint8_t kPosSourceIdInPayload{5};

template <typename PhysicalLayer = Physical, typename DatalinkLayer = Datalink<PhysicalLayer>>
class Network
{
public:
    Network(uint8_t own_id, DatalinkLayer &&datalink) : own_id_{own_id}, datalink_{std::forward<DatalinkLayer>(datalink)} {}

    CommunicationStatus Transmit(uint8_t to, Payload &payload) const;
    Payload ReceiveFrom(uint8_t from) const;

    virtual ~Network() = default;
    Network(const Network &other) = delete;
    Network(Network &&other) = delete;
    Network &operator=(const Network &other) = delete;
    Network &operator=(Network &&other) = delete;

private:
    uint8_t own_id_{};
    DatalinkLayer datalink_;
};