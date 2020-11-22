#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "crc.h"
#include "osi_layers/physical.h"
#include "osi_layers/application.h"

#include "unit/test_unit_base.h"
#include "utilities.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class Fixture1 : public ::testing::Test, public Application<>
{
protected:
    MOCK_METHOD(CommunicationStatus, transmitCredentials, (const uint8_t), (const, override));
};

TEST_F(Fixture1, TransmitCredentials_WhenTypical)
{
    auto expected = CommunicationStatus::InvalidCredentials;
    EXPECT_CALL(*this, transmitCredentials(_))
        .WillRepeatedly(Return(CommunicationStatus::InvalidCredentials)); // data

    auto actual = this->login(kOwnId, kPort);

    ASSERT_EQ(actual, expected);
}

class Fixture2 : public ::testing::Test, public Application<>
{
protected:
    MOCK_METHOD(CommunicationStatus, login, (const uint8_t, uint8_t), (const, override));
};

TEST_F(Fixture2, TransmitCredentials_WhenTypical)
{
    TString data_to_send{"abcd"};
    auto expected = CommunicationStatus::InvalidCredentials;
    EXPECT_CALL(*this, login(_, _))
        .WillRepeatedly(Return(CommunicationStatus::InvalidCredentials)); // data

    auto actual = this->Transmit(kOwnId, data_to_send);

    ASSERT_EQ(actual, expected);
}
