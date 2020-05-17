#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "osi_layers/datalink.h"
#include "crc.h"

constexpr uint8_t retransmitCountInCaseOfNoAcknowledge{0};

class Fixture : public ::testing::Test
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
    Datalink<> sut_{Physical{generic_transmit_byte, generic_receive_byte}};

    const static std::string send_data_;
    static std::string io_data_;
    static uint8_t pos_in_io_data_;
    static CRC crc_;
};

const std::string Fixture::send_data_{"abcd"};
std::string Fixture::io_data_{};
uint8_t Fixture::pos_in_io_data_{};
CRC Fixture::crc_;

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = crc_.append_crc_to_payload(payload);

    sut_.Transmit(payload, retransmitCountInCaseOfNoAcknowledge);
    pos_in_io_data_ = 0;
    auto actual = sut_.Receive();

    ASSERT_EQ(actual, expected);
}
TEST_F(Fixture, TRxFails_WhenBogusData)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = crc_.append_crc_to_payload(payload);

    sut_.Transmit(payload, retransmitCountInCaseOfNoAcknowledge);
    pos_in_io_data_ = 0;
    auto actual = sut_.Receive();
    actual.data[actual.size++] = 'A';

    ASSERT_FALSE(actual == expected);
}
