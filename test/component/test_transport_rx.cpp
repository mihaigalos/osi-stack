#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/transport.h"

#include "utilities.h"
#include "unit/test_unit_base.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class MockNetwork : public Network<>
{
public:
    MOCK_METHOD(Payload, Receive, (const uint8_t), (const, override));
    MOCK_METHOD(CommunicationStatus, Transmit, (const uint8_t to, Payload &payload), (const, override));
};

class Fixture : public ::testing::Test
{
public:
protected:
    Transport<MockNetwork> sut_{};
};

TEST_F(Fixture, ReturnsNothing_WhenSizeZero)
{
    TString expected{};
    EXPECT_CALL(sut_.network_, Receive(_))
        .WillOnce(Return(Payload{}));

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}
