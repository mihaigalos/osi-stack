#pragma once

#include <cstdint>

class CRC
{
public:
    CRC() = default;
    virtual ~CRC() = default;

    CRC(const CRC &other) = delete;
    CRC(CRC &&other) = delete;
    CRC &operator=(const CRC &other) = delete;
    CRC &operator=(CRC &&other) = delete;

    Payload append_crc_to_payload(const Payload &payload) const;
    TCRCChecksum computed_crc(const Payload &payload, uint8_t bytes_count) const;
    TCRCChecksum received_crc(const Payload &payload) const;
    bool crc_match(const Payload &payload) const;
    TCRCChecksum crc_update(TCRCChecksum initial, uint8_t current) const;
};