#include "osi_layers/physical.h"
#include "crc.h"

#include "utilities.h"

void Transceiver::Transmit(const Payload &payload) const
{
    log_dump_payload(payload, "Physical :: Transmit");
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        on_transmit_byte_(payload.data[i]);
    }
}

Payload Transceiver::Receive(const uint8_t expected_count) const
{
    auto payload = Payload{};

    for (uint8_t i = 0; i < expected_count; ++i)
    {
        payload.data[payload.size++] = on_receive_byte_();
    }
    log_dump_payload(payload, "Physical :: Receive");
    return payload;
}