#pragma once

#include <cstdint>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"

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
    Physical sut_{generic_transmit_byte, generic_receive_byte};

    static const std::string send_data_;
    static std::string io_data_;
    static uint8_t pos_in_io_data_;
};

const std::string IntegrationBase::send_data_{"abcd"};
std::string IntegrationBase::io_data_{};
uint8_t IntegrationBase::pos_in_io_data_{};