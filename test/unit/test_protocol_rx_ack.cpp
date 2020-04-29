#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "protocol.h"

#include "utilities.h"

constexpr uint8_t data_negative_acknowledge_[]{static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge)};
constexpr uint8_t data_acknowledge_[]{static_cast<uint8_t>(CommunicationStatus::Acknowledge)};

class Fixture : public ::testing::Test
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        static_cast<void>(payload);
    }

    static uint8_t generic_receive_byte()
    {
        return payloadified_data_.data[received_.size++];
    }

protected:
    virtual void SetUp() override
    {
        received_.Reset();

        payloadified_negative_acknowledge_ = Payload{data_negative_acknowledge_, 1};
        payloadified_acknowledge_ = Payload{data_acknowledge_, 1};

        payloadified_negative_acknowledge_ = append_crc_to_payload(payloadified_negative_acknowledge_);
        payloadified_acknowledge_ = append_crc_to_payload(payloadified_acknowledge_);

        payloadified_data_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
        received_.size = 0;
    }
    virtual void TearDown() override {}

    static Payload received_;
    UartHandshake<> sut_{generic_transmit_byte, generic_receive_byte};

    static std::string data_;
    static Payload payloadified_negative_acknowledge_;
    static Payload payloadified_acknowledge_;
    static Payload payloadified_data_;
};

Payload Fixture::received_{};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_negative_acknowledge_;
Payload Fixture::payloadified_acknowledge_;
Payload Fixture::payloadified_data_;

TEST_F(Fixture, ReceiveWithAcknowledgeWorks_WhenTypical)
{
    payloadified_data_ = append_crc_to_payload(payloadified_data_);
    auto expected = Payload(data_.c_str(), static_cast<uint8_t>(data_.length()));
    expected = append_crc_to_payload(expected);

    auto actual = sut_.ReceiveWithAcknowledge(expected.size);

    ASSERT_EQ(actual, expected);
}
