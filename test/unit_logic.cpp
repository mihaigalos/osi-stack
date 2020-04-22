#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "logic.h"

using ::testing::AtLeast;

class Fixture : public ::testing::Test
{
protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  auto MakeGenericHandshake()
  {
    auto generic_transmit_byte = [](const uint8_t &payload) {};
    auto generic_receive_byte = []() { return static_cast<uint8_t>('A'); };
    return UartHanshake{generic_transmit_byte, generic_receive_byte};
  }
};

TEST_F(Fixture, TransmitCalled_WhenTypical)
{
  auto sut_ = MakeGenericHandshake();
  uint8_t expected_byte = 'A';

  auto received_byte = sut_.Receive();

  ASSERT_EQ(expected_byte, received_byte);
}
