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
        return 0xFF;
    }

protected:
    virtual void SetUp() override
    {
        UnitBase::SetUp();
    }
    Session<> sut_{Transport<>{Network<>{kOwnId, {Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}}}};
};

TEST_F(Fixture, SetCookieSelfWorks_WhenTypical)
{
    auto expected = kCookieBaseValue;

    sut_.SetCookie(kCookieBaseValue);

    auto actual = sut_.own_cookie_;
    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, SetCookieOneClientWorks_WhenTypical)
{
    auto expected = kCookieBaseValue;

    sut_.SetCookie(kCookieBaseValue, 1);

    auto actual = sut_.clients_cookies_[1];
    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, SetCookieMultipleClientsWorks_WhenTypical)
{
    auto expected = kCookieBaseValue;

    sut_.SetCookie(kCookieBaseValue, 1);
    sut_.SetCookie(kCookieBaseValue, 2);

    auto actual = sut_.clients_cookies_[1];
    ASSERT_EQ(actual, expected);
    actual = sut_.clients_cookies_[2];
    ASSERT_EQ(actual, expected);
}
