#pragma once

#include <cstdint>

#include <containers/static_map.h>
#include "osi_layers/physical.h"

#include "config.h"

#include "network.h"

using TSegment = uint16_t;

template <typename NetworkLayer = Network<Datalink<Physical, CRC>>>
class Transport
{
public:
    Transport(NetworkLayer &&network) : network_{std::forward<NetworkLayer>(network)} {}

    CommunicationStatus Transmit(const uint8_t to, uint8_t *data, uint32_t size) const;
    uint32_t Receive(const uint8_t from, uint8_t *to) const;

    virtual ~Transport() = default;
    Transport(const Transport &other) = delete;
    Transport(Transport &&other) = delete;
    Transport &operator=(const Transport &other) = delete;
    Transport &operator=(Transport &&other) = delete;

private:
    NetworkLayer network_;
};