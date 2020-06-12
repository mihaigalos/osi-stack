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
    auto expected = LoginStatus::Success;

    auto actual = sut_.Login("myUser", "myPass");

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginInvalidCredentials_WhenInvalidPass)
{
    auto expected = LoginStatus::InvalidCredentials;

    auto actual = sut_.Login("myUser", "watch_this");

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginInvalidCredentials_WhenInvalidUser)
{
    auto expected = LoginStatus::InvalidCredentials;

    auto actual = sut_.Login("foo", "myPass");

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsLoggedIn_WhenTypical)
{
    auto expected{true};
    sut_.Login("myUser", "myPass");

    auto actual = sut_.IsLoggedIn();

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsNotLoggedIn_AfterLogout)
{
    auto expected{false};
    sut_.Login("myUser", "myPass");
    sut_.Logout();

    auto actual = sut_.IsLoggedIn();

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, CookieUpdated_WhenTypical)
{
    auto initial_cookie{sut_.cookie_};

    sut_.Login("myUser", "myPass");
    auto current_cookie{sut_.cookie_};

    ASSERT_NE(current_cookie, initial_cookie);
}

TEST_F(Fixture, CookieDeleted_WhenTypical)
{
    auto expected{decltype(sut_.cookie_){}};

    sut_.Login("myUser", "myPass");
    sut_.Logout();
    auto actual{sut_.cookie_};

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
    TString expected{"OK"};

    auto actual = sut_.attemptLogin(credentials);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, AttemptLoginFails_WhenFalseUser)
{
    TString user{"myFalseUser"}, pass{"myPass"};
    TString credentials = user + TString{" "} + pass;
    TString expected{"IC"};

    auto actual = sut_.attemptLogin(credentials);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, AttemptLoginFails_WhenFalsePass)
{
    TString user{"myUser"}, pass{"myFalsePass"};
    TString credentials = user + TString{" "} + pass;
    TString expected{"IC"};

    auto actual = sut_.attemptLogin(credentials);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginStatusToStringWorks_WhenError)
{
    TString expected{"ER"};

    auto actual = sut_.loginStatusToString(LoginStatus::Error);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginStatusToStringWorks_WhenInvalidCredentials)
{
    TString expected{"IC"};

    auto actual = sut_.loginStatusToString(LoginStatus::InvalidCredentials);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginStatusToStringWorks_WhenSuccess)
{
    TString expected{"OK"};

    auto actual = sut_.loginStatusToString(LoginStatus::Success);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginStatusToStringWorks_WhenUnknown)
{
    TString expected{"KO"};

    auto actual = sut_.loginStatusToString(LoginStatus::Unknown);

    ASSERT_EQ(actual, expected);
}