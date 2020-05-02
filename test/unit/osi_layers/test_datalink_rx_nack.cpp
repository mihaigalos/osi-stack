#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/datalink.h"

#include "utilities.h"

constexpr uint8_t data_negative_acknowledge_[]{static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge)};
constexpr uint8_t data_acknowledge_[]{static_cast<uint8_t>(CommunicationStatus::Acknowledge)};

class Fixture : public ::testing::Test
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        transmitted_.data[transmitted_.size++] = payload;
    }

    static uint8_t generic_receive_byte()
    {

        static uint8_t call_count{0};

        std::map<uint8_t, uint8_t> lookup_map{
            {0, payloadified_data_.size},
            {1, payloadified_data_.data[0]},
            {2, payloadified_data_.data[1] + 1},
            {3, payloadified_data_.data[2]},
            {4, payloadified_data_.data[3]},
            {5, payloadified_data_.data[4]},
            {6, payloadified_data_.data[5]},

            {7, payloadified_data_.size},
            {8, payloadified_data_.data[0]},
            {9, payloadified_data_.data[1]},
            {10, payloadified_data_.data[2]},
            {11, payloadified_data_.data[3]},
            {12, payloadified_data_.data[4]},
            {13, payloadified_data_.data[5]},
        };

        return lookup_map[call_count++];
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

TEST_F(Fixture, ReceiveWithNegativeAcknowledgeTransmissionWorks_WhenTypical)
{
    uint8_t nack_then_ack_with_crc[]{3, static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge),
                                     static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge),
                                     static_cast<uint8_t>(0),

                                     3, static_cast<uint8_t>(CommunicationStatus::Acknowledge),
                                     static_cast<uint8_t>(CommunicationStatus::Acknowledge),
                                     static_cast<uint8_t>(0)};

    auto expected = Payload{nack_then_ack_with_crc, sizeof(nack_then_ack_with_crc)};

    sut_.ReceiveWithAcknowledge();
    auto actual = transmitted_;

    log_dump_payload(actual, "actual");
    log_dump_payload(expected, "expected");

    ASSERT_EQ(expected, actual);
}
