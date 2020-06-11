#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/transport.h"

#include "utilities.h"
#include "unit/test_unit_base.h"

class Fixture : public UnitBase
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        transmitted_data_[pos_++] = payload;
    }
    static uint8_t generic_receive_byte()
    {
        return lookup_map_[call_count_++ % 4];
    }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();
        for (uint8_t i = 0; i < sizeof(transmitted_data_); ++i)
        {
            transmitted_data_[i] = 0;
        }
        pos_ = 0;
        call_count_ = 0;
        lookup_map_ = {
            {0, payload_size_byte_count + kCRCSize},
            {1, payloadified_acknowledge_.data[0]},
            {2, payloadified_acknowledge_.data[1]},
            {3, payloadified_acknowledge_.data[2]},
        };
    }
    Transport<> sut_{Network<>{kOwnId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}};

    static uint8_t transmitted_data_[255];
    static uint8_t pos_;
    static uint8_t call_count_;
};

uint8_t Fixture::transmitted_data_[255];
uint8_t Fixture::pos_;
uint8_t Fixture::call_count_;

TEST_F(Fixture, TransportTransmitWorks_WhenTypical)
{
    std::string data_to_transmit;
    for (uint8_t i = 'A'; i < 'z'; ++i)
    {
        data_to_transmit += static_cast<char>(i);
    }

    sut_.Transmit(kDestinationId, reinterpret_cast<uint8_t *>(const_cast<char *>(data_to_transmit.c_str())), data_to_transmit.length(), kPort);

    auto skip_past_metadata = kPayloadMaxSize - kTransportPayloadSize + kSizeofLength;
    uint8_t skip_past_metadata_count{0};
    for (uint8_t i = 0; i < 'z' - 'A'; ++i)
    {
        if (i > 0 && i % kTransportPayloadSize == 0)
        {
            ++skip_past_metadata_count;
        }
        ASSERT_EQ(transmitted_data_[i + kSizeofLength + skip_past_metadata_count * skip_past_metadata], 'A' + i);
    }
}

TEST_F(Fixture, TransportTransmitAcknowledge_WhenTypical)
{
    auto expected = CommunicationStatus::Acknowledge;
    std::string data_to_transmit;
    for (uint8_t i = 'A'; i < 'z'; ++i)
    {
        data_to_transmit += static_cast<char>(i);
    }

    auto actual = sut_.Transmit(kDestinationId, reinterpret_cast<uint8_t *>(const_cast<char *>(data_to_transmit.c_str())), data_to_transmit.length(), kPort);

    ASSERT_EQ(expected, actual);
}

TEST_F(Fixture, TransportTransmitAcknowledgeConstCharPointer_WhenTypical)
{
    auto expected = CommunicationStatus::Acknowledge;
    std::string data_to_transmit;
    for (uint8_t i = 'A'; i < 'z'; ++i)
    {
        data_to_transmit += static_cast<char>(i);
    }

    auto actual = sut_.Transmit(kDestinationId, data_to_transmit.c_str(), data_to_transmit.length(), kPort);

    ASSERT_EQ(expected, actual);
}
