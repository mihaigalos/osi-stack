#include "transceiver.h"
#include "config.h"
#include "crc.h"

inline CRCChecksum crc_function(CRCChecksum initial, uint8_t current)
{
    return initial + current;
}

CRCChecksum received_crc(const Payload &payload)
{
    CRCChecksum crc = 0;
    for (uint8_t i = 0; i < kCRCSize; ++i)
    {
        uint8_t current_crc_byte = payload.data[payload.size - kCRCSize + i];
        crc |= current_crc_byte << i * 8;
    }
    return crc;
}

CRCChecksum computed_crc(const Payload &payload, uint8_t bytes_count)
{
    CRCChecksum crc = 0;
    for (uint8_t i = 0; i < bytes_count; ++i)
    {
        crc = crc_function(crc, payload.data[i]);
    }
    return crc;
}

bool crc_match(const Payload &payload)
{
    if (payload.size <= kCRCSize)
    {
        return false;
    }
    return received_crc(payload) == computed_crc(payload, payload.size - kCRCSize);
}

Payload append_crc_to_payload(const Payload &payload)
{
    Payload result{payload};

    if (!crc_match(payload))
    {
        auto crc = computed_crc(payload, payload.size);

        for (uint8_t i = 0; i < kCRCSize; ++i)
        {
            uint8_t one_crc_byte = static_cast<uint8_t>(crc >> i * 8);
            result.data[result.size++] = one_crc_byte;
        }
    }
    return result;
}