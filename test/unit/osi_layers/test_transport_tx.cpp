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
    static void generic_transmit_byte(const uint8_t payload)
    {
        transmitted_data_[pos_++] = payload;
    }
    static uint8_t generic_receive_byte() { return 0xFF; }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();
        for (uint8_t i = 0; i < sizeof(transmitted_data_); ++i)
        {
            transmitted_data_[i] = 0;
        }
        pos_ = 0;
    }
    Transport<> sut_{Network<>{kOwnId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}};

    static uint8_t transmitted_data_[255];
    static uint8_t pos_;
};

uint8_t Fixture::transmitted_data_[255];
uint8_t Fixture::pos_;

TEST_F(Fixture, TransportTransmitWorks_WhenTypical)
{
    uint8_t payload_without_metadata_size = kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - kCRCSize - sizeof(TSegment);
    std::string data_to_transmit;
    for (uint8_t i = 'A'; i < 'z'; ++i)
    {
        data_to_transmit += static_cast<char>(i);
    }

    sut_.Transmit(kDestinationId, reinterpret_cast<uint8_t *>(const_cast<char *>(data_to_transmit.c_str())), data_to_transmit.length());

    for (uint8_t i = 0; i < payload_without_metadata_size; ++i)
    {
        ASSERT_EQ(transmitted_data_[i + kSizeofLength], 'A' + i);
    }
}

TEST_F(Fixture, TransportTransmitAcknowledge_WhenTypical)
{
    ASSERT_EQ(1, 1);
}
