#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include "osi_layers/physical.h"

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
  Transceiver sut_{generic_transmit_byte, generic_receive_byte};
};

uint8_t Fixture::data_;

// TEST_F(Fixture, ResetPayloadSizeWorks_WhenTypical)
// {
//   auto initial = Payload{"abc", 3};

//   initial.Reset();

//   ASSERT_EQ(initial.size, 0);
// }

TEST_F(Fixture, ResetPayloadDataWorks_WhenTypical)
{
  auto initial = Payload{"abc", 3};
  auto expected = Payload{};

  initial.Reset();

  ASSERT_EQ(initial, expected);
}
