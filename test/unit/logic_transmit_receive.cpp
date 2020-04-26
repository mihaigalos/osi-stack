#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
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
    return payloadified_.data[received_.size++];
  }

protected:
  virtual void SetUp() override
  {
    transmitted_.Reset();
    received_.Reset();

    payloadified_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
  }
  virtual void TearDown() override {}

  static Payload transmitted_, received_;
  UartIO sut_{generic_transmit_byte, generic_receive_byte};

  static std::string data_;
  static Payload payloadified_;
};

Payload Fixture::transmitted_{};
Payload Fixture::received_{};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_;

TEST_F(Fixture, TransmitWorks_WhenTypical)
{
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
  expected = append_crc_to_payload(expected);

  sut_.Transmit(payloadified_);

  ASSERT_EQ(transmitted_, expected);
}

TEST_F(Fixture, ReceiveWorks_WhenTypical)
{
  payloadified_ = append_crc_to_payload(payloadified_);
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
  expected = append_crc_to_payload(expected);

  received_ = sut_.Receive(static_cast<uint8_t>(data_.length() + kCRCSize));

  ASSERT_EQ(received_, expected);
}

TEST_F(Fixture, ReceiveCRCError_WhenCRCMismatch)
{
  payloadified_ = append_crc_to_payload(payloadified_);
  payloadified_.data[payloadified_.size - 1] = payloadified_.data[payloadified_.size - 1] + 2;
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  received_ = sut_.Receive(static_cast<uint8_t>(data_.length() + kCRCSize));

  ASSERT_FALSE(received_ == expected);
}

TEST_F(Fixture, ReceiveError_WhenIncorrectPayloadLength)
{
  auto payloadified = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() - 1)};
  payloadified_ = append_crc_to_payload(payloadified);
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  received_ = sut_.Receive(static_cast<uint8_t>(data_.length() + kCRCSize));

  ASSERT_FALSE(received_ == expected);
}

TEST_F(Fixture, ReceiveError_WhenForgotCRCAppend)
{
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(data_.length() + kCRCSize));

  ASSERT_FALSE(received_ == expected);
}
