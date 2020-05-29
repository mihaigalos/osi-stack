#include "osi_layers/physical.h"
#include "crc.h"

#include "utilities.h"

void Physical::Transmit(const Payload &payload) const
{
    log_dump_payload(payload, "Physical :: Transmit");

    on_transmit_byte_(payload.size);
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        on_transmit_byte_(payload.data[i]);
    }
}

Payload Physical::Receive() const
{
    auto payload = Payload{};
    auto expected_count = on_receive_byte_();

    if (expected_count <= kPayloadMaxSize)
    {
        for (uint8_t i = 0; i < expected_count; ++i)
        {
            payload.data[payload.size++] = on_receive_byte_();
        }
        log_dump_payload(payload, "Physical :: Receive");
        return payload;
    }
    else
    {
        log("Physical :: Receive expected_count > kPayloadMaxSize, impossible to receive " + std::to_string(expected_count) + " bytes.");
    }
    return {};
}