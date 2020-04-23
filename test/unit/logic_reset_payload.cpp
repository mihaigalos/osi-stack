#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "uart_handshake.h"

using ::testing::AtLeast;

class Fixture : public ::testing::Test
{

protected:
    virtual void SetUp() override {}
    virtual void TearDown() override {}
};

TEST_F(Fixture, ResetPayloadWorks_WhenTypical)
{
    auto initial = Payload{"abc", 3};
    auto expected = Payload{};

    initial.Reset();

    ASSERT_EQ(initial, expected);
}
