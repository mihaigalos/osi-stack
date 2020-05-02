#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"

class Fixture : public ::testing::Test
{
public:
protected:
    virtual void SetUp() override
    {
        payloadified_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
    }
    virtual void TearDown() override {}

    CRC sut_;
    static std::string data_;
    static Payload payloadified_;
};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_;

TEST_F(Fixture, CRCFunctionWorks_WhenTypical)
{
    CRCChecksum initial = 'a';
    CRCChecksum current = 'b';
    CRCChecksum expected = initial + current;

    auto actual = sut_.crc_update(initial, 'b');

    ASSERT_EQ(expected, actual);
}

TEST_F(Fixture, ComputeCRCWorks_WhenTypical)
{
    auto expected = sut_.computed_crc(payloadified_, payloadified_.size);

    ASSERT_EQ(expected, 'a' + 'b' + 'c' + 'd');
}

TEST_F(Fixture, ComputeCRCFails_WhenTypical)
{
    auto expected = sut_.computed_crc(payloadified_, payloadified_.size);

    ASSERT_FALSE(expected == 'a' + 'b' + 'c' + 'f');
}

TEST_F(Fixture, AppendCRCToPayloadWorks_WhenTypical)
{
    CRCChecksum computed_checksum = sut_.computed_crc(payloadified_, payloadified_.size);
    auto expected = payloadified_;
    expected.data[expected.size++] = static_cast<uint8_t>(computed_checksum);
    expected.data[expected.size++] = static_cast<uint8_t>(computed_checksum >> 8);

    payloadified_ = sut_.append_crc_to_payload(payloadified_);

    ASSERT_EQ(expected, payloadified_);
}

TEST_F(Fixture, ReceivedCRCWorks_WhenTypical)
{
    CRCChecksum expected = sut_.computed_crc(payloadified_, payloadified_.size);
    payloadified_ = sut_.append_crc_to_payload(payloadified_);

    CRCChecksum actual = sut_.received_crc(payloadified_);

    ASSERT_EQ(expected, actual);
}

TEST_F(Fixture, CRCMatch_WhenTypical)
{
    payloadified_ = sut_.append_crc_to_payload(payloadified_);

    auto is_crc_match = sut_.crc_match(payloadified_);

    ASSERT_TRUE(is_crc_match);
}

TEST_F(Fixture, CRCNotMatch_WhenTypical)
{
    payloadified_ = sut_.append_crc_to_payload(payloadified_);
    payloadified_.data[payloadified_.size - 1] = payloadified_.data[payloadified_.size - 1] + 2;

    auto is_crc_match = sut_.crc_match(payloadified_);

    ASSERT_FALSE(is_crc_match);
}
