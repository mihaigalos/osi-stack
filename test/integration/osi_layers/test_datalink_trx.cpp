#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "integration/test_integration_base.h"

class Fixture : public IntegrationBase
{
protected:
    Datalink<> sut_{Physical{generic_transmit_byte, generic_receive_byte}};
};

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};

    sut_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;
    sut_.Transmit(payload);
    pos_in_io_data_ = 0;
    auto received = sut_.Receive();

    ASSERT_TRUE(contains(received, payload));
}
TEST_F(Fixture, TRxFails_WhenBogusData)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};

    sut_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;
    sut_.Transmit(payload);
    pos_in_io_data_ = 0;
    auto received = sut_.Receive();
    received.data[0] = 'A';

    ASSERT_FALSE(contains(received, payload));
}
