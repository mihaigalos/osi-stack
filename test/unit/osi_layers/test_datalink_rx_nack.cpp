#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/datalink.h"

#include "utilities.h"
#include "test_unit_base.h"

class Fixture : public UnitBase
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        transmitted_.data[transmitted_.size++] = payload;
    }

    static uint8_t generic_receive_byte()
    {
        static uint8_t call_count{0};
        return lookup_map_[call_count++];
    }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();

        lookup_map_ = {
            {0, payloadified_data_with_crc_.size},
            {1, payloadified_data_with_crc_.data[0]},
            {2, payloadified_data_with_crc_.data[1] + 1},
            {3, payloadified_data_with_crc_.data[2]},
            {4, payloadified_data_with_crc_.data[3]},
            {5, payloadified_data_with_crc_.data[4]},
            {6, payloadified_data_with_crc_.data[5]},

            {7, payloadified_data_with_crc_.size},
            {8, payloadified_data_with_crc_.data[0]},
            {9, payloadified_data_with_crc_.data[1]},
            {10, payloadified_data_with_crc_.data[2]},
            {11, payloadified_data_with_crc_.data[3]},
            {12, payloadified_data_with_crc_.data[4]},
            {13, payloadified_data_with_crc_.data[5]},
        };
    }
    Datalink<> sut_{Physical{generic_transmit_byte, generic_receive_byte}};
};

TEST_F(Fixture, ReceiveWithNegativeAcknowledgeTransmissionWorks_WhenTypical)
{
    uint8_t nack_then_ack_with_crc[]{3, static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge),
                                     static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge),
                                     static_cast<uint8_t>(0),

                                     3, static_cast<uint8_t>(CommunicationStatus::Acknowledge),
                                     static_cast<uint8_t>(CommunicationStatus::Acknowledge),
                                     static_cast<uint8_t>(0)};

    auto expected = Payload{nack_then_ack_with_crc, sizeof(nack_then_ack_with_crc)};

    sut_.Receive();
    auto actual = transmitted_;

    log_dump_payload(actual, "actual");
    log_dump_payload(expected, "expected");

    ASSERT_EQ(expected, actual);
}
