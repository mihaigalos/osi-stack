#pragma once
#include <cstdint>

#include "osi_layers/physical.h"

#include "config.h"

#include "network.h"

template <typename NetworkLayer = Network<Datalink<Physical, CRC>>>
class Transport
{
public:
    Transport(NetworkLayer &&network) : network_{std::forward<NetworkLayer>(network)} {}
    Transport(Transport &&other) : network_{std::forward<NetworkLayer>(other.network_)} {}

    CommunicationStatus Transmit(const uint8_t to, const uint8_t *data, const uint32_t total_size, const uint8_t port) const;
    CommunicationStatus Transmit(const uint8_t to, const char *data, const uint32_t total_size, const uint8_t port) const;
    TString Receive(const uint8_t from_id, const uint8_t port) const;

    virtual ~Transport() = default;
    Transport(const Transport &other) = delete;

    Transport &operator=(const Transport &other) = delete;
    Transport &operator=(Transport &&other) = delete;

private:
    NetworkLayer network_;
};