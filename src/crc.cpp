#include "uart_handshake.h"
#include "config.h"
#include "crc.h"
#include <iostream>

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
        std::cout << "received_crc_byte: " << static_cast<int>(current_crc_byte) << std::endl;
    }

    std::cout << "---------------" << std::endl;
    std::cout << "received_crc: " << static_cast<int>(crc) << std::endl;
    return crc;
}

CRCChecksum computed_crc(const Payload &payload, uint8_t bytes_count)
{
    CRCChecksum crc = 0;
    for (uint8_t i = 0; i < bytes_count; ++i)
    {
        crc = crc_function(crc, payload.data[i]);
        std::cout << "computed_crc_byte: " << static_cast<int>(crc) << std::endl;
    }

    return crc;
}

bool crc_match(const Payload &payload)
{
    auto received = received_crc(payload);
    auto computed = computed_crc(payload, payload.size - kCRCSize);

    std::cout << std::hex << "received: " << received << std::endl;
    std::cout << std::hex << "computed: " << computed << std::endl;

    if (received == computed)
    {
        std::cout << "CRCs match. " << std::endl;
    }
    else
    {
        std::cout << "CRCs don't match. " << std::endl;
    }

    return received == computed;
}

Payload append_crc_to_payload(const Payload &payload)
{
    Payload result{payload};
    auto crc = computed_crc(payload, payload.size);
    std::cout << std::hex << "computed_crc: " << crc << std::endl;
    std::cout << "-------------" << std::endl;

    for (uint8_t i = 0; i < kCRCSize; ++i)
    {
        uint8_t one_crc_byte = static_cast<uint8_t>(crc >> i * 8);
        std::cout << "one_crc_byte: " << static_cast<unsigned int>(one_crc_byte) << std::endl;
        result.data[result.size++] = one_crc_byte;
    }
    return result;
}