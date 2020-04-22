#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "logic.h"

using ::testing::AtLeast;

class Fixture : public ::testing::Test
{
public:
  static void generic_transmit_byte(const uint8_t &payload)
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
};

Payload Fixture::transmitted_{};
Payload Fixture::received_{};
uint8_t Fixture::pos_in_received_{};

TEST_F(Fixture, TransmitCalled_WhenTypical)
{
  uint8_t transmitted_byte = {};
  auto sut_ = UartHanshake{generic_transmit_byte, generic_receive_byte};
  auto string = std::string{"abc"};
  auto payload_to_transmit = Payload{string.c_str(), string.length()};
  auto expected = payload_to_transmit;

  sut_.Transmit(payload_to_transmit);

  ASSERT_EQ(transmitted_, expected);
}
