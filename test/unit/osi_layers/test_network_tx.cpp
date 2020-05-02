#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/network.h"

#include "utilities.h"
#include "test_unit_base.h"

class Fixture : public UnitBase
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        if (transmitted_.size == kPosDestinationIdInPayload && payload == kOwnId)
        {
            to_field_matches_own_ = true;
        }
        transmitted_.data[transmitted_.size++] = payload;
    }

    static uint8_t generic_receive_byte()
    {
        if (to_field_matches_own_)
        {
            return lookup_map_[call_count_++];
        }
        return 0xFF;
    }

protected:
    virtual void SetUp() override
    {

        UnitBase::SetUp();
        call_count_ = 0;
        to_field_matches_own_ = false;
        lookup_map_ = {
            {0, payload_size_byte_count + kCRCSize},
            {1, payloadified_acknowledge_.data[0]},
            {2, payloadified_acknowledge_.data[1]},
            {3, payloadified_acknowledge_.data[2]},

            {4, payload_size_byte_count + kCRCSize},
            {5, payloadified_negative_acknowledge_.data[0]},
            {6, payloadified_negative_acknowledge_.data[1]},
            {7, payloadified_negative_acknowledge_.data[2]},
        };
    }
    Network<> sut_{kOwnId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}};
    static uint8_t call_count_;
    static bool to_field_matches_own_;
};

uint8_t Fixture::call_count_;
bool Fixture::to_field_matches_own_;

TEST_F(Fixture, NetworkTransmitToWorks_WhenTypical)
{

    sut_.TransmitTo(payloadified_data_, kDestinationId);

    ASSERT_EQ(transmitted_.data[kPosDestinationIdInPayload - 1], kDestinationId);
    ASSERT_EQ(transmitted_.data[kPosSourceIdInPayload - 1], kOwnId);
}

TEST_F(Fixture, NetworkTransmitToResultAcknowledge_WhenTypical)
{
    CommunicationStatus result = sut_.TransmitTo(payloadified_data_, kDestinationId);

    ASSERT_EQ(result, CommunicationStatus::Acknowledge);
}
