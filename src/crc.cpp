#include "logic.h"
#include "config.h"

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

Payload append_crc_to_payload(const Payload &payload)
{
    Payload result{payload};
    auto crc = computed_crc(payload);

    for (uint8_t i = 0; i < kCRCSize; ++i)
    {
        result.data[result.size++] = crc >> i;
    }
    return result;
}