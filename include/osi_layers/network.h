#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "datalink.h"

template <typename PhysicalLayer = Physical, typename CRCFunctions = CRC, typename DatalinkLayer = Datalink<Physical>>
class Network
{
public:
    Network(uint8_t own_id, DatalinkLayer &&datalink) : own_id_{own_id}, datalink_{std::forward<DatalinkLayer>(datalink)} {}

    CommunicationStatus TransmitTo(const Payload &payload, uint8_t to) const;
    Payload ReceiveFrom(uint8_t from) const;

    virtual ~Network() = default;
    Network(const Network &other) = delete;
    Network(Network &&other) = delete;
    Network &operator=(const Network &other) = delete;
    Network &operator=(Network &&other) = delete;

private:
    uint8_t own_id_{};
    CRCFunctions crc_;
    DatalinkLayer datalink_;
};