#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/session.h"

#include "utilities.h"
#include "unit/test_unit_base.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class MockTransport : public Transport<>
{
public:
    MOCK_METHOD(TString, Receive, (const uint8_t, const uint8_t), (const, override));
    MOCK_METHOD(CommunicationStatus, Transmit, (const uint8_t to, const char *data, const uint32_t total_size, const uint8_t port), (const, override));
};

class Fixture : public ::testing::Test
{
public:
protected:
    Session<MockTransport> sut_{{"User"}, {"Pass"}, kPort};
};

TEST_F(Fixture, ReceiveNoSendCookie_WhenCredentialsInvalid)
{
    TString expected{};
    expected += static_cast<char>(CommunicationStatus::InvalidCredentials);
    EXPECT_CALL(sut_.transport_, Receive(_, _))
        .WillOnce(Return("InvalidUser InvalidPass"));

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}