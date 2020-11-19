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

class Fixture : public UnitBase
{
public:
    static void generic_transmit_byte(const uint8_t payload)
    {
        static_cast<void>(payload);
    }

    static uint8_t generic_receive_byte()
    {
        static uint8_t call_count{0};

        if (call_count++ > 0)
        {
            return payloadified_data_with_segment_to_from_crc_.data[call_count - kSizeofLength - 1];
        }
        else
        {
            return payloadified_data_with_segment_to_from_crc_.size;
        }
    }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();
    }
    Session<> sut_{Transport<>{Network<>{kOwnId, {Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}}}, {"myUser"}, {"myPass"}, kPort};
};

TEST_F(Fixture, LoginSuccess_WhenTypical)
{
    auto expected = CommunicationStatus::Acknowledge;

    auto actual = sut_.Login("myUser", "myPass", kOwnId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginInvalidCredentials_WhenInvalidPass)
{
    auto expected = CommunicationStatus::InvalidCredentials;

    auto actual = sut_.Login("myUser", "watch_this", kOwnId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginInvalidCredentials_WhenInvalidUser)
{
    auto expected = CommunicationStatus::InvalidCredentials;

    auto actual = sut_.Login("foo", "myPass", kOwnId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsLoggedIn_WhenTypical)
{
    auto expected{true};
    sut_.Login("myUser", "myPass", kOwnId);

    auto actual = sut_.IsLoggedIn(kOwnId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsMultiLoggedIn_WhenTypical)
{
    auto expected1{true}, expected2{true};
    sut_.Login("myUser", "myPass", 1);
    sut_.Login("myUser", "myPass", 2);

    auto actual1 = sut_.IsLoggedIn(1);
    auto actual2 = sut_.IsLoggedIn(2);

    ASSERT_EQ(actual1, expected1);
    ASSERT_EQ(actual2, expected2);
}

TEST_F(Fixture, IsMultiSecondNotLoggedIn_WhenInvalidCreds)
{
    auto expected1{true}, expected2{false};
    sut_.Login("myUser", "myPass", 1);
    sut_.Login("invalid", "invalid", 2);

    auto actual1 = sut_.IsLoggedIn(1);
    auto actual2 = sut_.IsLoggedIn(2);

    ASSERT_EQ(actual1, expected1);
    ASSERT_EQ(actual2, expected2);
}

TEST_F(Fixture, IsNotLoggedIn_AfterLogout)
{
    auto expected{false};
    sut_.Login("myUser", "myPass", kOwnId);
    sut_.Logout();

    auto actual = sut_.IsSelfLoggedIn();

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, CookieUpdated_WhenTypical)
{
    sut_.clients_cookies_[kOwnId] = kCookieBaseValue;
    auto initial_cookie = sut_.clients_cookies_[kOwnId];

    sut_.Login("myUser", "myPass", kOwnId);
    auto current_cookie{sut_.clients_cookies_[kOwnId]};

    ASSERT_NE(current_cookie, initial_cookie);
}

TEST_F(Fixture, CookieDeleted_WhenTypical)
{
    auto expected{decltype(sut_.own_cookie_){}};

    sut_.Login("myUser", "myPass", kOwnId);
    sut_.Logout();
    auto actual{sut_.own_cookie_};

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, DeserializationUserPassWorks_WhenTypical)
{
    TString user{"myUser"}, pass{"myPass"};
    TString credentials = user + TString{" "} + pass;

    TString actual_user, actual_pass;
    sut_.deserializeUserPassword(credentials, actual_user, actual_pass);

    ASSERT_EQ(user, actual_user);
    ASSERT_EQ(pass, actual_pass);
}

TEST_F(Fixture, AttemptLoginWorks_WhenTypical)
{
    TString user{"myUser"}, pass{"myPass"};
    TString credentials = user + TString{" "} + pass;
    TString expected{};
    expected += static_cast<char>(CommunicationStatus::Acknowledge);

    auto actual = sut_.attemptLogin(credentials, kSourceId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, AttemptLoginFails_WhenFalseUser)
{
    TString user{"myFalseUser"}, pass{"myPass"};
    TString credentials = user + TString{" "} + pass;
    TString expected{};
    expected += static_cast<char>(CommunicationStatus::InvalidCredentials);

    auto actual = sut_.attemptLogin(credentials, kSourceId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, AttemptLoginFails_WhenFalsePass)
{
    TString user{"myUser"}, pass{"myFalsePass"};
    TString credentials = user + TString{" "} + pass;
    TString expected{};
    expected += static_cast<char>(CommunicationStatus::InvalidCredentials);

    auto actual = sut_.attemptLogin(credentials, kSourceId);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, SerializeCredentials_WhenTypical)
{
    TString expected{"myUser myPass"};

    auto actual = sut_.serializeUserPassword();

    ASSERT_EQ(actual, expected);
}
