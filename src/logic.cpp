#include "logic.h"

CRCChecksum crc_function(CRCChecksum initial, uint8_t current)
{
    return initial + current;
}

CRCChecksum received_crc(const Payload &payload)
{
    CRCChecksum crc = 0;
    for (uint8_t i = 0; i < kCRCSize; ++i)
    {
        crc |= payload.data[payload.size - kCRCSize + i] << i;
    }
    return crc;
}

CRCChecksum computed_crc(const Payload &payload)
{
    CRCChecksum crc = 0;
    for (uint8_t i = 0; i < payload.size - kCRCSize; ++i)
    {
        crc = crc_function(crc, payload.data[i]);
    }
    return crc;
}

bool crc_match(const Payload &payload)
{
    return received_crc(payload) == computed_crc(payload);
}

inline Payload append_crc_to_payload(const Payload &payload)
{
    Payload result{payload};
    auto crc = computed_crc(payload);

    for (uint8_t i = 0; i < kCRCSize; ++i)
    {
        result.data[result.size++] = crc >> i;
    }
    return result;
}

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