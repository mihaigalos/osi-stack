#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "integration/test_integration_base.h"

constexpr uint8_t retransmitCountInCaseOfNoAcknowledge{0};

class Fixture : public IntegrationBase
{
protected:
    Transport<> sut1_{Network<>{kFromId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}};
    Transport<> sut2_{Network<>{kDestinationId, Datalink<>{Physical{generic_transmit_byte, generic_receive_byte}}}};
};

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    sut1_.network_.datalink_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut2_.network_.datalink_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut1_.Transmit(kDestinationId, send_data_.c_str(), send_data_.size());
    pos_in_io_data_ = 0;

    auto received = sut2_.Receive(kFromId);
    auto actual = std::string{received.c_str()};

    ASSERT_EQ(actual, send_data_);
}

TEST_F(Fixture, TRxFails_WhenBogusData)
{
    sut1_.network_.datalink_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut2_.network_.datalink_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut1_.Transmit(kDestinationId, send_data_.c_str(), send_data_.size());
    pos_in_io_data_ = 0;

    auto received = sut2_.Receive(kFromId);
    auto actual = std::string{received.c_str()};
    actual[0] = '\x0A';

    ASSERT_FALSE(actual == send_data_);
}

TEST_F(Fixture, TRxWorks_WhenPayloadLengthLongerThanPayloadMaxSize)
{
    std::string initial_data{"abcdefghijklmnopqrstuvwxyz"};
    sut1_.network_.datalink_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;
    sut2_.network_.datalink_.retransmit_count_ = retransmitCountInCaseOfNoAcknowledge;

    sut1_.Transmit(kDestinationId, initial_data.c_str(), initial_data.size());
    pos_in_io_data_ = 0;

    auto received = sut2_.Receive(kFromId);
    auto actual = std::string{received.c_str()};
    ASSERT_EQ(actual, initial_data);
}
