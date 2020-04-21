#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <iostream>
#include <string>

#include "logic.h"

using ::testing::AtLeast;

Payload on_receive()
{
  std::string data_string{"HelloWorld"};
  return Payload{data_string.c_str(), static_cast<uint8_t>(data_string.length())};
}

class Fixture : public ::testing::Test
{

protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(Fixture, TransmitCalled_WhenTypical)
{
  UartHanshake sut_{[](const Payload &payload) {}, []() {
                      std::string data_string{"HelloWorld"};
                      return Payload{data_string.c_str(), static_cast<uint8_t>(data_string.length())}; }};
  std::string expected_string = "HelloWorld";
  auto expected_payload = Payload{expected_string.c_str(), static_cast<uint8_t>(expected_string.length())};

  auto received_payload = sut_.receive();

  ASSERT_EQ(expected_payload, received_payload);
}
