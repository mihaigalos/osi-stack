#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"
#include "crc.h"

class Fixture : public ::testing::Test
{
public:
  static void generic_transmit_byte(const uint8_t payload) { static_cast<void>(payload); }
  static uint8_t generic_receive_byte() { return 0xFF; }

protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  Physical sut_{generic_transmit_byte, generic_receive_byte};
};

TEST_F(Fixture, EmptyPayloadWorks_WhenSizeOfDataToReceiveGreaterThanMaxPayloadSize)
{
  auto expected = Payload{};

  Payload actual = sut_.Receive();

  ASSERT_EQ(actual, expected);
}