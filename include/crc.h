#pragma once

#include <cstdint>

Payload append_crc_to_payload(const Payload &payload);
CRCChecksum computed_crc(const Payload &payload, uint8_t bytes_count);
CRCChecksum received_crc(const Payload &payload);
bool crc_match(const Payload &payload);