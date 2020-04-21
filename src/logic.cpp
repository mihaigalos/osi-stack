#include "logic.h"

void UartHanshake::Transmit(const Payload &payload)
{
    on_transmit_(payload);
}

Payload UartHanshake::Receive()
{
    return on_receive_();
}