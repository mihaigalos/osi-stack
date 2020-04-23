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
    return data_;
  }

protected:
  virtual void SetUp() override { data_ = {}; }
  virtual void TearDown() override {}

  static uint8_t data_;
  UartHanshake sut_{generic_transmit_byte, generic_receive_byte};
};

uint8_t Fixture::data_;

TEST_F(Fixture, ReceiveWorks_WhenTypical)
{
  uint8_t expected_byte = 'A';
  data_ = expected_byte;

  auto received_byte = sut_.Receive();

  ASSERT_EQ(received_byte, expected_byte);
}

TEST_F(Fixture, TransmitWorks_WhenTypical)
{
  uint8_t transmitted_byte = {};
  auto payload_to_transmit = Payload{};
  payload_to_transmit.data[0] = 'B';
  payload_to_transmit.size = 1;

  sut_.Transmit(payload_to_transmit);
  transmitted_byte = data_;

  ASSERT_EQ(payload_to_transmit, transmitted_byte);
}

TEST_F(Fixture, TransmitReceiveWorks_WhenTypical)
{
  uint8_t transmitted_byte = {};
  uint8_t data_ = 'C';
  auto expected = data_;

  sut_.Transmit(sut_.Receive());

  ASSERT_EQ(expected, data_);
}
