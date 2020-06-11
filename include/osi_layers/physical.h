#pragma once

#include <cstdint>
#include "config.h"
#include "payload.h"
#include "utilities.h"

using TVoidUint8 = void (*)(const uint8_t);
using TUint8Void = uint8_t (*)();

class Physical
{
public:
  Physical(TVoidUint8 on_transmit, TUint8Void on_receive) : on_transmit_byte_{on_transmit}, on_receive_byte_{on_receive} {}
  Physical(Physical &&other) : on_transmit_byte_{other.on_transmit_byte_}, on_receive_byte_{other.on_receive_byte_} {}

  void Transmit(const Payload &payload) const
  {
    log_dump_physical(payload, "Physical :: Transmit");

    on_transmit_byte_(payload.size);
    for (uint8_t i = 0; i < payload.size; ++i)
    {
      on_transmit_byte_(payload.data[i]);
    }
  }

  Payload Receive() const
  {
    auto payload = Payload{};
    auto expected_count = on_receive_byte_();

    if (expected_count <= kPayloadMaxSize)
    {
      for (uint8_t i = 0; i < expected_count; ++i)
      {
        payload.data[payload.size++] = on_receive_byte_();
      }
      log_dump_physical(payload, "Physical :: Receive");
      return payload;
    }
    else
    {
      log("Physical :: Receive expected_count > kPayloadMaxSize, impossible to receive " + std::to_string(expected_count) + " bytes.");
    }
    return {};
  }

#ifdef TESTING
  Physical()
  {
  }
#endif

  virtual ~Physical() = default;
  Physical(const Physical &other) = delete;
  Physical &operator=(const Physical &other) = delete;
  Physical &operator=(Physical &&other) = delete;

private:
  TVoidUint8 on_transmit_byte_;
  TUint8Void on_receive_byte_;
};