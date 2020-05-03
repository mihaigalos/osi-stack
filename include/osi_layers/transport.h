#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "network.h"

template <typename NetworkLayer = Network<Datalink<Physical, CRC>>>
class Transport
{
    Transport(NetworkLayer &&network) : network_{std::forward<NetworkLayer>(network)} {}

    CommunicationStatus TransmitWithSplit(uint8_t to, uint8_t *data, uint32_t size) const;
    Payload ReceiveWithSplit(uint8_t from) const;

    virtual ~Transport() = default;
    Transport(const Transport &other) = delete;
    Transport(Transport &&other) = delete;
    Transport &operator=(const Transport &other) = delete;
    Transport &operator=(Transport &&other) = delete;

private:
    NetworkLayer network_;
};