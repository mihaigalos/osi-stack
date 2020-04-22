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
    data_ = payload;
  }

  static uint8_t generic_receive_byte()
  {
    return static_cast<uint8_t>(data_);
  }

protected:
  virtual void SetUp() override { data_ = {}; }
  virtual void TearDown() override {}

  static uint8_t data_;
};

uint8_t Fixture::data_;

TEST_F(Fixture, ReceiveCalled_WhenTypical)
{
  auto sut_ = UartHanshake{Fixture::generic_transmit_byte, Fixture::generic_receive_byte};
  uint8_t expected_byte = 'A';
  data_ = expected_byte;

  auto received_byte = sut_.Receive();

  ASSERT_EQ(expected_byte, received_byte);
}

TEST_F(Fixture, TransmitCalled_WhenTypical)
{
  uint8_t transmitted_byte = {};
  auto sut_ = UartHanshake{generic_transmit_byte, generic_receive_byte};
  uint8_t byte_to_transmit = 'B';

  sut_.Transmit(byte_to_transmit);
  transmitted_byte = data_;

  ASSERT_EQ(transmitted_byte, byte_to_transmit);
}

TEST_F(Fixture, TransmitReceive_WhenTypical)
{
  uint8_t transmitted_byte = {};
  auto sut_ = UartHanshake{generic_transmit_byte, generic_receive_byte};
  uint8_t data_ = 'C';

  sut_.Transmit(sut_.Receive());
  transmitted_byte = data_;

  ASSERT_EQ(transmitted_byte, data_);
}
