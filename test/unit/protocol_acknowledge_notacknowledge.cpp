#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "uartio.h"
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
        static uint8_t call_count = 0;

        if (++call_count <= 1 + kCRCSize)
        {
            return payloadified_negative_acknowledge_.data[received_.size++];
        }
        else if (call_count == 1 + kCRCSize + 1)
        {
            received_.size = 0;
        }

        return payloadified_acknowledge_.data[received_.size++];
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
        received_.size = 0;
    }
    virtual void TearDown() override {}

    static Payload transmitted_, received_;
    UartHandshake<> sut_{generic_transmit_byte, generic_receive_byte};

    static std::string data_;
    static Payload payloadified_negative_acknowledge_;
    static Payload payloadified_acknowledge_;
    static Payload payloadified_data_;
};

Payload Fixture::transmitted_{};
Payload Fixture::received_{};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_negative_acknowledge_;
Payload Fixture::payloadified_acknowledge_;
Payload Fixture::payloadified_data_;

TEST_F(Fixture, TransmitWorks_WhenTypical)
{

    CommunicationStatus result = sut_.TransmitWithAcknowledge(payloadified_data_);

    ASSERT_EQ(result, CommunicationStatus::Acknowledge);
}
