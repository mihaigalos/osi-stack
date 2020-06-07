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
    Session<> sut_{Transport<>{Network<>{kOwnId, {Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}}}, {"mihai"}, {"galos"}, kPort};
};

TEST_F(Fixture, LoginSuccess_WhenTypical)
{
    auto expected = LoginStatus::Success;

    auto actual = sut_.Login("mihai", "galos");

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginInvalidCredentials_WhenInvalidPass)
{
    auto expected = LoginStatus::InvalidCredentials;

    auto actual = sut_.Login("mihai", "watch_this");

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, LoginInvalidCredentials_WhenInvalidUser)
{
    auto expected = LoginStatus::InvalidCredentials;

    auto actual = sut_.Login("foo", "galos");

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsLoggedIn_WhenTypical)
{
    auto expected{true};
    sut_.Login("mihai", "galos");

    auto actual = sut_.IsLoggedIn();

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsNotLoggedIn_AfterLogout)
{
    auto expected{false};
    sut_.Login("mihai", "galos");
    sut_.Logout();

    auto actual = sut_.IsLoggedIn();

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, CookieUpdated_WhenTypical)
{
    auto initial_cookie{sut_.cookie_};

    sut_.Login("mihai", "galos");
    auto current_cookie{sut_.cookie_};

    ASSERT_NE(current_cookie, initial_cookie);
}

TEST_F(Fixture, CookieDeleted_WhenTypical)
{
    auto expected{decltype(sut_.cookie_){}};

    sut_.Login("mihai", "galos");
    sut_.Logout();
    auto actual{sut_.cookie_};

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, DeserealizationUserPassWorks_WhenTypical)
{
    TString user{"mihai"}, pass{"galos"};
    TString credentials = user + TString{" "} + pass;

    TString actual_user, actual_pass;
    sut_.deserializeUserPassword(credentials, actual_user, actual_pass);

    ASSERT_EQ(user, actual_user);
    ASSERT_EQ(pass, actual_pass);
}