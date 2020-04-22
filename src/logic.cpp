#include "logic.h"

void UartHanshake::Transmit(const uint8_t &payload)
{
    on_transmit_byte_(payload);
}

uint8_t UartHanshake::Receive()
{
    return on_receive_byte_();
}