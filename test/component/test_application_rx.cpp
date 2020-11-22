#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/application.h"

#include "utilities.h"
#include "unit/test_unit_base.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class MockPresentation : public Presentation<>
{
public:
    MOCK_METHOD(TString, Receive, (const uint8_t, const uint8_t), (const, override));
    MOCK_METHOD(CommunicationStatus, Transmit, (const uint8_t to, const uint8_t port, TString &data), (const, override));
};

class Fixture : public ::testing::Test
{
public:
protected:
    Application<MockPresentation> sut_{{"User"}, {"Pass"}, kPort};
};

TEST_F(Fixture, LoginSuccess_WhenTypical)
{
    TString expected;
    expected += static_cast<char>(CommunicationStatus::Acknowledge);
    expected += " ";
    expected += kCookieBaseValueStringified;
    EXPECT_CALL(sut_.presentation_, Receive(_, _))
        .WillOnce(Return("User Pass"));

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginSuccessTransmitCookie_WhenTypical)
{
    TString expected;
    expected += static_cast<char>(CommunicationStatus::Acknowledge);
    expected += " ";
    expected += kCookieBaseValueStringified;
    TString actual{};
    EXPECT_CALL(sut_.presentation_, Receive(_, _))
        .WillOnce(Return("User Pass"));

    ON_CALL(sut_.presentation_, Transmit(_, _, _)).WillByDefault(Invoke([&](const uint8_t to, const uint8_t port, TString &data) {
        static_cast<void>(to);
        static_cast<void>(port);

        actual = data;

        return CommunicationStatus::Acknowledge;
    }));

    sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, ReceiveWorks_WhenLoggedIn)
{
    TString expected = "abcdef";
    EXPECT_CALL(sut_.presentation_, Receive(_, _))
        .WillOnce(Return("abcdef"));
    sut_.presentation_.session_.clients_cookies_[kSourceId] = kCookieBaseValue;

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}
