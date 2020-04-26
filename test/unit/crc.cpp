#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "uart_handshake.h"
#include "crc.h"

using ::testing::AtLeast;

class Fixture : public ::testing::Test
{
public:
protected:
    virtual void SetUp() override
    {
        payloadified_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
    }
    virtual void TearDown() override {}

    static std::string data_;
    static Payload payloadified_;
};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_;

TEST_F(Fixture, ComputeCRCWorks_WhenTypical)
{
    auto expected = computed_crc(payloadified_, payloadified_.size);

    ASSERT_EQ(expected, 'a' + 'b' + 'c' + 'd');
}

TEST_F(Fixture, ComputeCRCFails_WhenTypical)
{
    auto expected = computed_crc(payloadified_, payloadified_.size);

    ASSERT_FALSE(expected == 'a' + 'b' + 'c' + 'f');
}