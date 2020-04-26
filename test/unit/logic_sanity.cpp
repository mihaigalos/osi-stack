#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "uart_handshake.h"

using ::testing::AtLeast;

class Fixture : public ::testing::Test
{
public:
  static void generic_transmit_byte(const uint8_t payload)
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
  UartIO sut_{generic_transmit_byte, generic_receive_byte};
};

uint8_t Fixture::data_;

TEST_F(Fixture, ReceiveDefaultPayload_WhenExpectedCountGreaterThanMaxPayloadSize)
{
  auto expected = Payload{};

  auto actual = sut_.Receive(kPayloadMaxSize + 1);

  ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, ResetPayloadWorks_WhenTypical)
{
  auto initial = Payload{"abc", 3};
  auto expected = Payload{};

  initial.Reset();

  ASSERT_EQ(initial, expected);
}
