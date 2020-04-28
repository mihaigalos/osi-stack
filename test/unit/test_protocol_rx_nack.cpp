#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "transceiver.h"
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
        transmitted_.data[transmitted_.size++] = payload;
        if (static_cast<CommunicationStatus>(payload) == CommunicationStatus::NegativeAcknowledge)
        {
            received_.size = 0;
        }
    }

    static uint8_t generic_receive_byte()
    {

        uint8_t invalid_data = 0xAA;
        uint8_t invalid_byte_position = 2;

        if (++call_count_ == invalid_byte_position)
        {
            received_.size++;
            return invalid_data;
        }
        return payloadified_data_.data[received_.size++];
    }

protected:
    virtual void SetUp() override
    {
        transmitted_.Reset();
        received_.Reset();

        payloadified_negative_acknowledge_ = Payload{data_negative_acknowledge_, 1};
        payloadified_acknowledge_ = Payload{data_acknowledge_, 1};

        payloadified_negative_acknowledge_ = append_crc_to_payload(payloadified_negative_acknowledge_);
        payloadified_acknowledge_ = append_crc_to_payload(payloadified_acknowledge_);

        payloadified_data_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
        payloadified_data_ = append_crc_to_payload(payloadified_data_);

        received_.size = 0;
        call_count_ = 0;
    }
    virtual void TearDown() override {}

    static Payload received_;
    UartHandshake<> sut_{generic_transmit_byte, generic_receive_byte};
    static std::string data_;
    static Payload payloadified_negative_acknowledge_;
    static Payload payloadified_acknowledge_;
    static Payload payloadified_data_;
    static Payload transmitted_;
    static uint8_t call_count_;
};

Payload Fixture::received_{};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_negative_acknowledge_;
Payload Fixture::payloadified_acknowledge_;
Payload Fixture::payloadified_data_;

Payload Fixture::transmitted_{};
uint8_t Fixture::call_count_;

TEST_F(Fixture, ReceiveWithNegativeAcknowledgeWorks_WhenTypical)
{

    auto expected = Payload(data_.c_str(), static_cast<uint8_t>(data_.length()));
    expected = append_crc_to_payload(expected);

    auto actual = sut_.ReceiveWithAcknowledge(expected.size);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, ReceiveWithNegativeAcknowledgeTransmissionWorks_WhenTypical)
{
    uint8_t nack_then_ack_with_crc[]{static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge),
                                     static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge),
                                     static_cast<uint8_t>(0),
                                     static_cast<uint8_t>(CommunicationStatus::Acknowledge),
                                     static_cast<uint8_t>(CommunicationStatus::Acknowledge),
                                     static_cast<uint8_t>(0)};

    auto expected = Payload{nack_then_ack_with_crc, 6};

    sut_.ReceiveWithAcknowledge(data_.length() + kCRCSize);
    auto actual = transmitted_;

    log_dump_payload(actual, "actual");
    log_dump_payload(expected, "expected");

    ASSERT_EQ(expected, actual);
}
