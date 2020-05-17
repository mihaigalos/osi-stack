#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "osi_layers/physical.h"
#include "crc.h"

#include "integration/test_integration_base.h"

class Fixture : public IntegrationBase
{
protected:
    Physical sut_{generic_transmit_byte, generic_receive_byte};
};

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = payload;

    sut_.Transmit(payload);
    auto actual = sut_.Receive();

    ASSERT_EQ(actual, expected);
}
TEST_F(Fixture, TRxFails_WhenBogusData)
{
    auto payload = Payload{send_data_.c_str(), static_cast<uint8_t>(send_data_.length())};
    auto expected = payload;

    sut_.Transmit(payload);
    auto actual = sut_.Receive();
    actual.data[actual.size++] = 'A';

    ASSERT_FALSE(actual == expected);
}
