#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/datalink.h"

#include "utilities.h"

constexpr uint8_t payload_size_byte_count{1};
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
        return lookup_map_[call_count++];
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

        lookup_map_ = {
            {0, payload_size_byte_count + kCRCSize},
            {1, payloadified_negative_acknowledge_.data[0]},
            {2, payloadified_negative_acknowledge_.data[1]},
            {3, payloadified_negative_acknowledge_.data[2]},

            {4, payload_size_byte_count + kCRCSize},
            {5, payloadified_acknowledge_.data[0]},
            {6, payloadified_acknowledge_.data[1]},
            {7, payloadified_acknowledge_.data[2]},
        };
    }
    virtual void TearDown() override {}

    static Payload received_;
    UartHandshake<> sut_{generic_transmit_byte, generic_receive_byte};

    static std::string data_;
    static Payload payloadified_negative_acknowledge_;
    static Payload payloadified_acknowledge_;
    static Payload payloadified_data_;
    static std::map<uint8_t, uint8_t> lookup_map_;
};

Payload Fixture::received_{};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_negative_acknowledge_;
Payload Fixture::payloadified_acknowledge_;
Payload Fixture::payloadified_data_;

std::map<uint8_t, uint8_t> Fixture::lookup_map_;

TEST_F(Fixture, TransmitWithAcknowledgeWorks_WhenTypical)
{
    CommunicationStatus result = sut_.TransmitWithAcknowledge(payloadified_data_);

    ASSERT_EQ(result, CommunicationStatus::Acknowledge);
}
