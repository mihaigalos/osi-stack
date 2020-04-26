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

TEST_F(Fixture, AppendCRCToPayloadWorks_WhenTypical)
{
    CRCChecksum computed_checksum = computed_crc(payloadified_, payloadified_.size);
    auto expected = payloadified_;
    expected.data[expected.size++] = static_cast<uint8_t>(computed_checksum);
    expected.data[expected.size++] = static_cast<uint8_t>(computed_checksum >> 8);

    payloadified_ = append_crc_to_payload(payloadified_);

    ASSERT_EQ(expected, payloadified_);
}

TEST_F(Fixture, ReceivedCRCWorks_WhenTypical)
{
    CRCChecksum expected = computed_crc(payloadified_, payloadified_.size);
    payloadified_ = append_crc_to_payload(payloadified_);

    CRCChecksum actual = received_crc(payloadified_);

    ASSERT_EQ(expected, actual);
}
