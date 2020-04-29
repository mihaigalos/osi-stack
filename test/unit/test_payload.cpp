#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "payload.h"

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

TEST_F(Fixture, PayloadAssignmentWorks_WhenTypical)
{
    Payload actual{payloadified_};

    Payload expected = actual;

    ASSERT_EQ(expected, actual);
}

TEST_F(Fixture, PayloadOperatorEqualsWorks_WhenTypical)
{
    Payload actual{payloadified_};

    Payload expected = actual;

    ASSERT_TRUE(expected == actual);
}

TEST_F(Fixture, PayloadConstructionFromConstCharWorks_WhenTypical)
{
    std::string expected{"abcd"};

    bool is_match = true;
    for (uint8_t i = 0; i < payloadified_.size; ++i)
    {
        if (expected[i] != payloadified_.data[i])
        {
            is_match = false;
            break;
        }
    }

    ASSERT_TRUE(is_match);
}
