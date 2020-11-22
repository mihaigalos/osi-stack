#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "crc.h"
#include "osi_layers/physical.h"
#include "osi_layers/application.h"

#include "unit/test_unit_base.h"
#include "utilities.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class MockPresentation : public Presentation<>
{
public:
    MOCK_METHOD(TString, Receive, (const uint8_t, const uint8_t), (const, override));
    MOCK_METHOD(CommunicationStatus, Transmit, (const uint8_t to, const uint8_t port, TString &data), (const, override));
};

class Fixture : public ::testing::Test
{
public:
protected:
    Application<MockPresentation> sut_{{"User"}, {"Pass"}, kPort};
};

TEST_F(Fixture, TransmitCredentials_WhenTypical)
{
    auto expected = CommunicationStatus::Acknowledge;
    TString acknowledege_with_cookie;
    acknowledege_with_cookie += static_cast<char>(CommunicationStatus::Acknowledge);
    acknowledege_with_cookie += " ";
    acknowledege_with_cookie += kCookieBaseValueStringified;

    EXPECT_CALL(sut_.presentation_, Transmit(_, _, _))
        .WillOnce(
            Return(CommunicationStatus::NoAcknowledgeRequired))    // credentials
        .WillRepeatedly(Return(CommunicationStatus::Acknowledge)); // data
    EXPECT_CALL(sut_.presentation_, Receive(_, _)).WillOnce(Return(acknowledege_with_cookie));

    TString to_send{"abcd"};
    sut_.presentation_.session_.transport_.network_.datalink_.retransmit_count_ =
        kRetransmitCountInCaseOfNoAcknowledge;

    auto actual = sut_.Transmit(kDestinationId, to_send);

    ASSERT_EQ(actual, expected);
}
