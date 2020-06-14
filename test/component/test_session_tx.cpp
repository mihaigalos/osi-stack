#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "crc.h"
#include "osi_layers/physical.h"
#include "osi_layers/session.h"

#include "unit/test_unit_base.h"
#include "utilities.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class MockTransport : public Transport<>
{
public:
    MOCK_METHOD(TString, Receive, (const uint8_t, const uint8_t),
                (const, override));
    MOCK_METHOD(CommunicationStatus, Transmit,
                (const uint8_t to, const char *data, const uint32_t total_size,
                 const uint8_t port),
                (const, override));
};

class Fixture : public ::testing::Test
{
public:
protected:
    Session<MockTransport> sut_{{"User"}, {"Pass"}, kPort};
};

TEST_F(Fixture, TransmitCredentials_WhenTypical)
{
    auto expected = CommunicationStatus::Acknowledge;
    TString acknowledege_with_cookie;
    acknowledege_with_cookie += static_cast<char>(CommunicationStatus::Acknowledge);
    acknowledege_with_cookie += " \xBE\xEF";

    EXPECT_CALL(sut_.transport_, Transmit(_, _, _, _))
        .WillOnce(
            Return(CommunicationStatus::NoAcknowledgeRequired))    // credentials
        .WillRepeatedly(Return(CommunicationStatus::Acknowledge)); // data
    EXPECT_CALL(sut_.transport_, Receive(_, _)).WillOnce(Return(acknowledege_with_cookie));

    TString to_send{"abcd"};
    sut_.transport_.network_.datalink_.retransmit_count_ =
        kRetransmitCountInCaseOfNoAcknowledge;

    auto actual = sut_.Transmit(kDestinationId, to_send);

    ASSERT_EQ(actual, expected);
}
