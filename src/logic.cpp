#include "logic.h"

void UartHanshake::Transmit(const Payload &payload)
{
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        on_transmit_byte_(payload.data[i]);
    }
}

Payload UartHanshake::Receive(uint8_t expected_count)
{
    auto return_value = Payload{};
    if (expected_count < kPayloadMaxSize)
    {
        return_value.data[return_value.size++] = on_receive_byte_();
    }
    return return_value;
}