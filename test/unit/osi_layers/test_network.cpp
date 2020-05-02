#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/network.h"

#include "utilities.h"
#include "test_unit_base.h"

constexpr uint8_t kOwnId{0x01};
constexpr uint8_t kDestinationId{0x02};

class Fixture : public UnitBase
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        transmitted_.data[transmitted_.size++] = payload;
    }

    static uint8_t generic_receive_byte()
    {
        return lookup_map_[call_count_++];
    }

protected:
    virtual void SetUp() override
    {

        UnitBase::SetUp();
        call_count_ = 0;
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
    Network<> sut_{kOwnId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}};
    static uint8_t call_count_;
};

uint8_t Fixture::call_count_;

TEST_F(Fixture, NetworkTransmitToResultAcknowledge_WhenTypical)
{
    CommunicationStatus result = sut_.TransmitTo(payloadified_data_, kDestinationId);

    ASSERT_EQ(result, CommunicationStatus::Acknowledge);
}

TEST_F(Fixture, NetworkTransmitToWorks_WhenTypical)
{
    uint8_t pos_destination_id_in_payload{5};
    uint8_t pos_source_id_in_payload{6};

    sut_.TransmitTo(payloadified_data_, kDestinationId);

    ASSERT_EQ(transmitted_.data[pos_destination_id_in_payload], kDestinationId);
    ASSERT_EQ(transmitted_.data[pos_source_id_in_payload], kOwnId);
}
