#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/application.h"

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
            return payloadified_tstring_data_with_segment_to_from_crc_.data[call_count_ - kSizeofLength - 1];
        }
        else
        {
            return payloadified_tstring_data_with_segment_to_from_crc_.size;
        }
    }

protected:
    virtual void SetUp() override
    {
        call_count_ = 0;

        tstring_data_ = {"User"};
        tstring_data_ += " ";
        tstring_data_ += "Pass";

        if (isOddLength(tstring_data_))
        {
            tstring_data_ += '\0';
        }

        sut_.presentation_.encdec_.encrypt(sut_.presentation_.encryptionRounds_, kEncryptionKey, reinterpret_cast<uint8_t *>(const_cast<char *>(tstring_data_.c_str())), tstring_data_.size());
        auto j = tstring_data_.size();
        (void)j;
        UnitBase::SetUp();
    }
    static uint8_t call_count_;
    Application<> sut_{Presentation<>{Session<>{Transport<>{Network<>{kOwnId, {Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}}}}, kEncryptionRounds}, {"User"}, {"Pass"}, kPort};
};

uint8_t Fixture::call_count_{};

TEST_F(Fixture, LoginSuccess_WhenTypical)
{
    TString expected;
    expected += static_cast<char>(CommunicationStatus::Acknowledge);
    expected += " ";
    expected += kCookieBaseValueStringified;
    sut_.presentation_.session_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}

// TEST_F(Fixture, CookieDeseralizeWorks_WhenTypical)
// {
//     decltype(sut_.own_cookie_) expected{kCookieBaseValue};
//     sut_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

//     auto response = sut_.Receive(kSourceId, kPort);
//     auto actual = sut_.deserializeCookie(response);

//     ASSERT_EQ(actual, expected);
// }

// TEST_F(Fixture, CookieDeseralizeFails_WhenTypical)
// {
//     decltype(sut_.own_cookie_) expected{0xAAAA};
//     sut_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

//     auto response = sut_.Receive(kSourceId, kPort);
//     auto actual = sut_.deserializeCookie(response);

//     ASSERT_NE(actual, expected);
// }