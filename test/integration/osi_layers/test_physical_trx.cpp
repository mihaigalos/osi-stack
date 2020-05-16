#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "osi_layers/physical.h"
#include "crc.h"

class Fixture : public ::testing::Test
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        receive_data_ += static_cast<char>(payload);
    }

    static uint8_t generic_receive_byte()
    {

        return receive_data_.at(pos_++);
    }

protected:
    virtual void SetUp() override
    {
        send_data_ = {};
        receive_data_ = {};
        pos_ = {};
    }
    virtual void TearDown() override {}

    static Payload transmitted_, received_;
    Physical sut_{generic_transmit_byte, generic_receive_byte};

    static std::string send_data_, receive_data_;
    static uint8_t pos_;
};

std::string Fixture::send_data_{"abcd"};
std::string Fixture::receive_data_{};
uint8_t Fixture::pos_{};

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = payload;

    sut_.Transmit(payload);
    auto actual = sut_.Receive();

    ASSERT_EQ(actual, expected);
}
TEST_F(Fixture, TRxFails_WhenBogusData)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = payload;

    sut_.Transmit(payload);
    auto actual = sut_.Receive();
    actual.data[actual.size++] = 'A';

    ASSERT_FALSE(actual == expected);
}
