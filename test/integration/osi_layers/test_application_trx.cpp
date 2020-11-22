#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <mutex>
#include <string>
#include <array>

#include "integration/test_integration_base.h"

#include <thread>

using namespace std::chrono_literals;
bool thread2_transmit_finished{false};
std::mutex mutex;

class Fixture : public IntegrationBase
{

public:
    void SetUp() override
    {
        pos_in_io_data_ = 0;
        io_data_.clear();
        IntegrationBase::SetUp();
    }

protected:
    static void generic_transmit_byte1(const uint8_t payload)
    {
        mutex.lock();
        io_data_ += static_cast<char>(payload);
        mutex.unlock();
    }

    static uint8_t generic_receive_byte1()
    {
        while (!thread2_transmit_finished)
        {
            std::this_thread::sleep_for(20ms);
        }
        mutex.lock();

        static uint8_t call_count{0};

        char character = io_data_[pos_in_io_data_++];

        ResetChannel(call_count);
        mutex.unlock();

        return static_cast<uint8_t>(character);
    }

    static void generic_transmit_byte2(const uint8_t payload)
    {
        mutex.lock();
        io_data_ += static_cast<char>(payload);
        static uint8_t call_count{0};
        uint8_t total_payload_size{static_cast<uint8_t>(io_data_[0])};

        if (call_count++ == total_payload_size)
        {
            thread2_transmit_finished = true;
            pos_in_io_data_ = 0;
        }
        mutex.unlock();
    }

    static uint8_t generic_receive_byte2()
    {
        mutex.lock();
        char character = io_data_[pos_in_io_data_++];

        static uint8_t call_count{0};
        ResetChannel(call_count);
        mutex.unlock();
        return static_cast<uint8_t>(character);
    }

    Application<> sut1_{Presentation<>{Session<>{Transport<>{Network<>{kFromId, Datalink<>{Physical{generic_transmit_byte1, generic_receive_byte1}}}}}, kEncryptionRounds}, {"User"}, {"Pass"}, kPort};
    Application<> sut2_{Presentation<>{Session<>{Transport<>{Network<>{kFromId, Datalink<>{Physical{generic_transmit_byte1, generic_receive_byte1}}}}}, kEncryptionRounds}, {"User"}, {"Pass"}, kPort};

private:
    static void ResetChannel(uint8_t &call_count)
    {
        uint8_t total_payload_size{static_cast<uint8_t>(io_data_[0])};

        if (call_count++ == total_payload_size)
        {
            io_data_.clear();
            pos_in_io_data_ = 0;
            call_count = 0;
        }
    }
};

TEST_F(Fixture, DISABLED_TRxWorks_WhenTypical)
{
    TString actual{};
    TString expected{"abcde"};
    sut1_.presentation_.session_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;
    sut2_.presentation_.session_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

    std::thread t1([&] { sut1_.Transmit(kDestinationId, expected); });
    std::thread t2([&] { std::this_thread::sleep_for(50ms);actual = sut2_.Receive(kFromId, kPort); });
    t1.join();
    t2.join();
    actual = sut2_.Receive(kFromId, kPort);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, DISABLED_TRxFails_WhenInvalidCredentials)
{
    Application<> sut1{Presentation<>{Session<>{Transport<>{Network<>{kFromId, Datalink<>{Physical{generic_transmit_byte1, generic_receive_byte1}}}}}, kEncryptionRounds}, {"User"}, {"Pass"}, kPort};
    TString to_transmit{"abcde"};
    CommunicationStatus actual{CommunicationStatus::Unknown};
    CommunicationStatus expected{CommunicationStatus::SessionCookieError};

    sut1.presentation_.session_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;
    sut2_.presentation_.session_.transport_.network_.datalink_.retransmit_count_ = kRetransmitCountInCaseOfNoAcknowledge;

    std::thread t1([&] { actual = sut1.Transmit(kDestinationId, to_transmit); });
    std::thread t2([&] { std::this_thread::sleep_for(50ms);sut2_.Receive(kFromId, kPort); });
    t1.join();
    t2.join();

    ASSERT_EQ(actual, expected);
}
