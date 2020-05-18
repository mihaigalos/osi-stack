#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "integration/test_integration_base.h"

constexpr uint8_t retransmitCountInCaseOfNoAcknowledge{0};

class Fixture : public IntegrationBase
{
protected:
    Datalink<> sut_{Physical{generic_transmit_byte, generic_receive_byte}};
};

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = crc_.append_crc_to_payload(payload);

    sut_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut_.Transmit(payload);
    pos_in_io_data_ = 0;
    auto actual = sut_.Receive();

    ASSERT_EQ(actual, expected);
}
TEST_F(Fixture, TRxFails_WhenBogusData)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = crc_.append_crc_to_payload(payload);

    sut_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut_.Transmit(payload);
    pos_in_io_data_ = 0;
    auto actual = sut_.Receive();
    actual.data[actual.size++] = 'A';

    ASSERT_FALSE(actual == expected);
}
