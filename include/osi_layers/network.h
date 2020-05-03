#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "datalink.h"

constexpr uint8_t kPosDestinationIdInPayload{4};
constexpr uint8_t kPosSourceIdInPayload{5};

template <typename PhysicalLayer = Physical, typename CRCFunctions = CRC, typename DatalinkLayer = Datalink<Physical>>
class Network
{
public:
    Network(uint8_t own_id, DatalinkLayer &&datalink) : own_id_{own_id}, datalink_{std::forward<DatalinkLayer>(datalink)} {}
    Network(uint8_t own_id, const DatalinkLayer &datalink) : own_id_{own_id}, datalink_{datalink} {}

    virtual CommunicationStatus TransmitTo(Payload &payload, uint8_t to) const;
    virtual Payload ReceiveFrom(uint8_t from) const;

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