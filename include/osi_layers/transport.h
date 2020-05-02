#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "datalink.h"

template <typename PhysicalLayer = Physical, typename CRCFunctions = CRC, typename DatalinkLayer = Datalink<Physical>>
class Transport
{
public:
    Transport(uint8_t own_id, DatalinkLayer &&datalink) : own_id_{own_id}, datalink_{std::forward<DatalinkLayer>(datalink)} {}

    CommunicationStatus TransmitTo(const Payload &payload, uint8_t to) const;
    Payload ReceiveFrom(uint8_t from) const;

    virtual ~Transport() = default;
    Transport(const Transport &other) = delete;
    Transport(Transport &&other) = delete;
    Transport &operator=(const Transport &other) = delete;
    Transport &operator=(Transport &&other) = delete;

private:
    uint8_t own_id_{};
    CRCFunctions crc_;
    DatalinkLayer datalink_;
};