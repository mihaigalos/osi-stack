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
        static_cast<void>(payload);
    }

    static uint8_t generic_receive_byte()
    {
        static uint8_t call_count{0};

        if (call_count++ > 0)
        {
            return payloadified_data_with_to_from_crc_.data[call_count - 2];
        }
        else
        {
            return payloadified_data_with_to_from_crc_.size;
        }
    }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();
    }
    Network<> sut_{kOwnId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}};
};

TEST_F(Fixture, ReceiveFromWorks_WhenTypical)
{
    auto actual = sut_.ReceiveFrom(kSourceId);

    ASSERT_NE(actual.size, 0);
    ASSERT_EQ(kOwnId, actual.data[kPosDestinationIdInPayload]);
}
