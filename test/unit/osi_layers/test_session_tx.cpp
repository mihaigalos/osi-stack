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
        if (call_count_++ > 0)
        {
            return payloadified_data_with_segment_to_from_crc_.data[call_count_ - kSizeofLength - 1];
        }
        else
        {
            return payloadified_data_with_segment_to_from_crc_.size;
        }
    }
    static void onCookieReceived(CommunicationStatus received_cookie_communication_status)
    {
        cookie_status_ = received_cookie_communication_status;
    }

protected:
    virtual void SetUp() override
    {
        cookie_status_ = {};
        call_count_ = 0;
        data_ = std::string{"User"} + " " + "Pass";
        UnitBase::SetUp();
    }
    static uint8_t call_count_;
    static CommunicationStatus cookie_status_;
    Session<> sut_{Transport<>{Network<>{kOwnId, {Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}}}, {"User"}, {"Pass"}, kPort, onCookieReceived};
};

uint8_t Fixture::call_count_{};
CommunicationStatus Fixture::cookie_status_{};

TEST_F(Fixture, OnCookieReceivedCalled_WhenTypical)
{
    auto expected{CommunicationStatus::NoAcknowledgeRequired};
    TString to_transmit{"abcdef"};
    sut_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

    sut_.Transmit(kDestinationId, to_transmit);

    auto actual = cookie_status_;
    ASSERT_EQ(actual, expected);
}