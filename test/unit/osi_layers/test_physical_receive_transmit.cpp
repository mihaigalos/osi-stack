#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"

class Fixture : public ::testing::Test
{
public:
  static void generic_transmit_byte(const uint8_t payload)
  {
    if (!transmitted_.size)
    {
      transmitted_.size = payload;
    }
    else
    {
      transmitted_.data[pos_in_transmission_++] = payload;
    }
  }

  static uint8_t generic_receive_byte()
  {
    if (!pos_in_reception_)
    {
      pos_in_reception_++;
      return payloadified_.size;
    }
    else
    {
      auto result = payloadified_.data[pos_in_reception_ - 1];
      pos_in_reception_++;
      return result;
    }
  }

protected:
  virtual void SetUp() override
  {
    transmitted_.Reset();
    received_.Reset();

    payloadified_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
    pos_in_transmission_ = {};
    pos_in_reception_ = {};
  }
  virtual void TearDown() override {}

  static Payload transmitted_, received_;
  Transceiver sut_{generic_transmit_byte, generic_receive_byte};

  static std::string data_;
  static Payload payloadified_;
  static uint8_t pos_in_transmission_, pos_in_reception_;
};

Payload Fixture::transmitted_{};
Payload Fixture::received_{};

std::string Fixture::data_{"abcd"};
Payload Fixture::payloadified_;
uint8_t Fixture::pos_in_transmission_, Fixture::pos_in_reception_;

TEST_F(Fixture, TransmitWorks_WhenTypical)
{
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};

  sut_.Transmit(payloadified_);

  ASSERT_EQ(transmitted_, expected);
}

TEST_F(Fixture, ReceiveWorks_WhenTypical)
{
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};

  received_ = sut_.Receive();

  ASSERT_EQ(received_, expected);
}

TEST_F(Fixture, ReceiveCRCError_WhenCRCMismatch)
{
  payloadified_.data[payloadified_.size - 1] = payloadified_.data[payloadified_.size - 1] + 42;
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};

  received_ = sut_.Receive();

  ASSERT_FALSE(received_ == expected);
}

TEST_F(Fixture, ReceiveError_WhenIncorrectPayloadLength)
{
  payloadified_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() - 1)};
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};

  received_ = sut_.Receive();

  ASSERT_FALSE(received_ == expected);
}

TEST_F(Fixture, ReceiveError_WhenForgotCRCAppend)
{
  auto expected = Payload{data_.c_str(), static_cast<uint8_t>(data_.length() + kCRCSize)};

  sut_.Receive();

  ASSERT_FALSE(received_ == expected);
}
