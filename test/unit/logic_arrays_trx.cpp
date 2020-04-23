#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "logic.h"
#include "crc.h"

using ::testing::AtLeast;

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
  static uint8_t pos_in_received_, pos_in_transmitted_;
  UartHanshake sut_{generic_transmit_byte, generic_receive_byte};
};

Payload Fixture::transmitted_{};
Payload Fixture::received_{};
uint8_t Fixture::pos_in_received_{};
uint8_t Fixture::pos_in_transmitted_{};

TEST_F(Fixture, TransmitWorks_WhenTypical)
{
  auto string = std::string{"a"};

  auto payload_to_transmit = Payload{string.c_str(), static_cast<uint8_t>(string.length())};
  auto expected = Payload{string.c_str(), static_cast<uint8_t>(string.length() + kCRCSize)};

  sut_.Transmit(payload_to_transmit);

  ASSERT_EQ(transmitted_, expected);
}

TEST_F(Fixture, ReceiveWorks_WhenTypical)
{
  auto string = std::string{"abcd"};
  auto payloadified = Payload{string.c_str(), static_cast<uint8_t>(string.length())};
  received_ = append_crc_to_payload(payloadified);

  auto expected = Payload{string.c_str(), static_cast<uint8_t>(string.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(string.length()));

  ASSERT_EQ(received_, expected);
}

TEST_F(Fixture, ReceiveCRCError_WhenCRCMismatch)
{
  auto string = std::string{"abcd"};
  auto payloadified_without_crc_update = Payload{string.c_str(), static_cast<uint8_t>(string.length())};
  received_ = Payload{payloadified_without_crc_update};

  auto expected = Payload{string.c_str(), static_cast<uint8_t>(string.length() + kCRCSize)};

  sut_.Receive(static_cast<uint8_t>(string.length()));

  ASSERT_FALSE(received_ == expected);
}
