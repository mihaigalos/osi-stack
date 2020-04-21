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
    auto generic_transmit = [](const Payload &payload) {};
    auto generic_receive = []() {
                      std::string data_string{"HelloWorld"};
                      return Payload{data_string.c_str(), static_cast<uint8_t>(data_string.length())}; };
    return UartHanshake{generic_transmit, generic_receive};
  }
};

TEST_F(Fixture, TransmitCalled_WhenTypical)
{
  auto sut_ = MakeGenericHandshake();
  std::string expected_string = "HelloWorld";
  auto expected_payload = Payload{expected_string.c_str(), static_cast<uint8_t>(expected_string.length())};

  auto received_payload = sut_.Receive();

  ASSERT_EQ(expected_payload, received_payload);
}
