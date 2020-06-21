#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <array>

#include "integration/test_integration_base.h"

#include <thread>

using namespace std::chrono_literals;
bool hasCookie{false};
class Fixture : public IntegrationBase
{

public:
    void SetUp() override
    {
        index_in_channel_ = 0;
        channel_.clear();
        IntegrationBase::SetUp();
    }

protected:
    static void generic_transmit_byte1(const uint8_t payload)
    {
        channel_ += static_cast<char>(payload);
    }

    static uint8_t generic_receive_byte1()
    {
        while (!hasCookie)
        {
            std::this_thread::sleep_for(20ms);
        }

        static uint8_t call_count{0};

        char character = channel_[index_in_channel_++];

        ResetChannel(call_count);

        return static_cast<uint8_t>(character);
    }

    static void generic_transmit_byte2(const uint8_t payload)
    {
        channel_ += static_cast<char>(payload);
        static uint8_t call_count{0};
        uint8_t total_payload_size{static_cast<uint8_t>(channel_[0])};

        if (call_count++ == total_payload_size)
        {
            hasCookie = true;
            index_in_channel_ = 0;
        }
    }

    static uint8_t generic_receive_byte2()
    {
        char character = channel_[index_in_channel_++];

        static uint8_t call_count{0};
        ResetChannel(call_count);
        return static_cast<uint8_t>(character);
    }

    Session<> sut1_{Transport<>{Network<>{kFromId, Datalink<>{Physical{generic_transmit_byte1, generic_receive_byte1}}}}, {"User"}, {"Pass"}, kPort};
    Session<> sut2_{Transport<>{Network<>{kDestinationId, Datalink<>{Physical{generic_transmit_byte2, generic_receive_byte2}}}}, {"User"}, {"Pass"}, kPort};
    static std::string channel_;
    static uint8_t index_in_channel_;

private:
    static void ResetChannel(uint8_t &call_count)
    {
        uint8_t total_payload_size{static_cast<uint8_t>(channel_[0])};

        if (call_count++ == total_payload_size)
        {
            channel_.clear();
            index_in_channel_ = 0;
            call_count = 0;
        }
    }
};

std::string Fixture::channel_;
uint8_t Fixture::index_in_channel_;

TEST_F(Fixture, TRxWorks_WhenTypical)
{
    TString actual{};
    TString expected{"abcde"};
    sut1_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;
    sut2_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

    std::thread t1([&] { sut1_.Transmit(kDestinationId, expected); });
    std::thread t2([&] { actual = sut2_.Receive(kFromId, kPort); });
    t1.join();
    t2.join();
    actual = sut2_.Receive(kFromId, kPort);

    ASSERT_EQ(actual, expected);
}
