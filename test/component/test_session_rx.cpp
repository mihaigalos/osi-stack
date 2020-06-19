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

TEST_F(Fixture, LoginSuccess_WhenTypical)
{
    TString expected;
    expected += static_cast<char>(CommunicationStatus::Acknowledge);
    expected += " ";
    expected += kCookieBaseValue;
    EXPECT_CALL(sut_.transport_, Receive(_, _))
        .WillOnce(Return("User Pass"));

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginSuccessTransmitCookie_WhenTypical)
{
    TString expected;
    expected += static_cast<char>(CommunicationStatus::Acknowledge);
    expected += " ";
    expected += kCookieBaseValue;
    TString actual{};
    EXPECT_CALL(sut_.transport_, Receive(_, _))
        .WillOnce(Return("User Pass"));

    ON_CALL(sut_.transport_, Transmit(_, _, _, _)).WillByDefault(Invoke([&](const uint8_t to, const char *data, const uint32_t total_size, const uint8_t port) {
        static_cast<void>(to);
        static_cast<void>(port);

        for (uint8_t i = 0; i < total_size - 1; ++i)
        {
            actual += data[i];
        }

        return CommunicationStatus::Acknowledge;
    }));

    sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, ReceiveWorks_WhenLoggedIn)
{
    TString expected = "abcdef";
    EXPECT_CALL(sut_.transport_, Receive(_, _))
        .WillOnce(Return("abcdef"));
    sut_.cookie_ = 0xBEEF;

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}