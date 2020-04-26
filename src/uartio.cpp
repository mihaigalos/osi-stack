#include "uartio.h"
#include "crc.h"

#include "utilities.h"

void UartIO::Transmit(const Payload &payload) const
{
    auto payload_with_crc = append_crc_to_payload(payload);
    log_dump_payload(payload, "transmitting payload");
    for (uint8_t i = 0; i < payload_with_crc.size; ++i)
    {
        on_transmit_byte_(payload_with_crc.data[i]);
    }
}

Payload UartIO::Receive(const uint8_t expected_count) const
{
    auto payload = Payload{};

    if (expected_count < kPayloadMaxSize)
    {
        for (uint8_t i = 0; i < expected_count; ++i)
        {
            payload.data[payload.size++] = on_receive_byte_();
        }
        log_dump_payload(payload, "received payload");
        if (crc_match(payload))
        {
            return payload;
        }
    }
    return {};
}