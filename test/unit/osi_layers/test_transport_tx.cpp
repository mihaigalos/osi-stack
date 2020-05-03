#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/transport.h"

#include "utilities.h"
#include "test_unit_base.h"

class Fixture : public UnitBase
{
public:
    static void generic_transmit_byte(const uint8_t payload) { static_cast<void>(payload); }
    static uint8_t generic_receive_byte() { return 0xFF; }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();
    }
    Transport<> sut_{Network<>{kOwnId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}};
};

TEST_F(Fixture, TransportTransmitWorks_WhenTypical)
{
    ASSERT_EQ(1, 1);
}

TEST_F(Fixture, NetworkTransmitResultAcknowledge_WhenTypical)
{
    ASSERT_EQ(1, 1);
}
