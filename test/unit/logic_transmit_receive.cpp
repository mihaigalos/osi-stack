#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "uart_handshake.h"
#include "crc.h"

class Fixture : public ::testing::Test
{
public:
  static void generic_transmit_byte(const uint8_t payload)
  {
    transmitted_.data[transmitted_.size++] = payload;
  }

  static uint8_t generic_receive_byte()
  {
    return received_.data[pos_in_received_++];
  }

protected:
  virtual void SetUp() override
  {
    transmitted_.Reset();
    received_.Reset();
    pos_in_received_ = {};
  }
  virtual void TearDown() override {}

  static Payload transmitted_, received_;
  static uint8_t pos_in_received_;
  UartHanshake sut_{generic_transmit_byte, generic_receive_byte};

  std::string data_{"abcd"};
  Payload payloadified_{data_.c_str(), static_cast<uint8_t>(data_.length())};
};

Payload Fixture::transmitted_{};
Payload Fixture::received_{};
uint8_t Fixture::pos_in_received_{};

TEST_F(Fixture, TransmitWorks_WhenTypical)
{
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Transmit(payloadified_);

  ASSERT_EQ(transmitted_, expected);
}

TEST_F(Fixture, ReceiveWorks_WhenTypical)
{
  received_ = append_crc_to_payload(payloadified_);
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(data_.length()));

  ASSERT_EQ(received_, expected);
}

TEST_F(Fixture, ReceiveCRCError_WhenCRCMismatch)
{
  received_ = payloadified_;
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(data_.length()));

  ASSERT_FALSE(received_ == expected);
}

TEST_F(Fixture, ReceiveError_WhenIncorrectPayloadLength)
{
  auto payloadified = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() - 1)};
  received_ = append_crc_to_payload(payloadified);
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(data_.length()));

  ASSERT_FALSE(received_ == expected);
}

TEST_F(Fixture, ReceiveError_WhenForgotCRCAppend)
{
  received_ = payloadified_;
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(data_.length()));

  ASSERT_FALSE(received_ == expected);
}
