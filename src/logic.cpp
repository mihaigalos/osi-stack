#include "logic.h"

void UartHanshake::Transmit(const Payload &payload)
{
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        on_transmit_byte_(payload.data[i]);
    }
}

Payload UartHanshake::Receive()
{
    auto received_value = on_receive_byte_();
    auto return_value = Payload{};

    return_value.data[0] = received_value;
    return_value.size = 1;

    return return_value;
}