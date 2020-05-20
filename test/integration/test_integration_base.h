#pragma once

#include <cstdint>
#include <string>

#define private public // For testing purposes only

#include "osi_layers/transport.h"
#include "osi_layers/network.h"
#include "osi_layers/datalink.h"
#include "osi_layers/physical.h"

#include "crc.h"

constexpr uint8_t kFromId{0x01};
constexpr uint8_t kDestinationId{0x02};

bool contains(const Payload &a, const Payload &b)
{
    bool result{false};
    if (b.size < a.size)
    {
        for (uint8_t i = 0; i < b.size; ++i)
        {
            if (a.data[i] != b.data[i])
            {
                return false;
            }
        }
        result = true;
    }
    return result;
}

class IntegrationBase : public ::testing::Test
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        io_data_ += static_cast<char>(payload);
    }

    static uint8_t generic_receive_byte()
    {
        return io_data_.at(pos_in_io_data_++);
    }

protected:
    virtual void SetUp() override
    {
        io_data_ = {};
        pos_in_io_data_ = {};
    }
    virtual void TearDown() override {}

    static Payload transmitted_, received_;

    static const std::string send_data_;
    static std::string io_data_;
    static uint8_t pos_in_io_data_;
    static CRC crc_;
};

const std::string IntegrationBase::send_data_{"abcd"};
std::string IntegrationBase::io_data_{};
uint8_t IntegrationBase::pos_in_io_data_{};
CRC IntegrationBase::crc_;