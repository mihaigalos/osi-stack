#pragma once

#include <cstdint>

class CRC
{
public:
    Payload append_crc_to_payload(const Payload &payload) const;
    CRCChecksum computed_crc(const Payload &payload, uint8_t bytes_count) const;
    CRCChecksum received_crc(const Payload &payload) const;
    bool crc_match(const Payload &payload) const;
    CRCChecksum crc_update(CRCChecksum initial, uint8_t current) const;
};