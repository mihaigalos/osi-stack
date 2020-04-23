#include "uart_handshake.h"
#include "crc.h"

void UartHanshake::Transmit(const Payload &payload)
{
    auto payload_with_crc = append_crc_to_payload(payload);
    for (uint8_t i = 0; i < payload_with_crc.size; ++i)
    {
        on_transmit_byte_(payload_with_crc.data[i]);
    }
}

Payload UartHanshake::Receive(uint8_t expected_count)
{
    auto payload = Payload{};
    if (expected_count < kPayloadMaxSize)
    {
        for (uint8_t i = 0; i < expected_count; ++i)
        {
            payload.data[payload.size++] = on_receive_byte_();
        }

        if (crc_match(payload))
        {
            return payload;
        }
    }
    return {};
}