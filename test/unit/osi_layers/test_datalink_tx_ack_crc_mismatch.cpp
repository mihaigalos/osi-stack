#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/datalink.h"

#include "utilities.h"
#include "unit/test_unit_base.h"

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
            {0, payload_size_byte_count + kCRCSize},
            {1, payloadified_crc_mismatch_.data[0]},
            {2, payloadified_crc_mismatch_.data[1]},
            {3, payloadified_crc_mismatch_.data[2]},

            {4, payload_size_byte_count + kCRCSize},
            {5, payloadified_crc_mismatch_.data[0]},
            {6, payloadified_crc_mismatch_.data[1]},
            {7, payloadified_crc_mismatch_.data[2]},

            {8, payload_size_byte_count + kCRCSize},
            {9, payloadified_crc_mismatch_.data[0]},
            {10, payloadified_crc_mismatch_.data[1]},
            {11, payloadified_crc_mismatch_.data[2]},
        };
    }

    Datalink<> sut_{Physical{generic_transmit_byte, generic_receive_byte}};
};

TEST_F(Fixture, DatalinkTransmitErrorCRCMismatch_WhenTypical)
{
    CommunicationStatus result = sut_.Transmit(payloadified_data_);

    ASSERT_EQ(result, CommunicationStatus::CRCMismatch);
}
