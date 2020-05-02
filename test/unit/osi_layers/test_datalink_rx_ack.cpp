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
        static_cast<void>(payload);
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
            {2, payloadified_data_with_crc_.data[1]},
            {3, payloadified_data_with_crc_.data[2]},
            {4, payloadified_data_with_crc_.data[3]},
            {5, payloadified_data_with_crc_.data[4]},
            {6, payloadified_data_with_crc_.data[5]},
        };
    }
    Datalink<> sut_{generic_transmit_byte, generic_receive_byte};
};

TEST_F(Fixture, ReceiveWithAcknowledgeWorks_WhenTypical)
{
    auto expected = Payload(data_.c_str(), static_cast<uint8_t>(data_.length()));
    expected = append_crc_to_payload(expected);

    auto actual = sut_.ReceiveWithAcknowledge();

    ASSERT_EQ(actual, expected);
}
