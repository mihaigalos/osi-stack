#pragma once

#include <cstdint>
#include "config.h"
#include "payload.h"

using TVoidUint8 = void (*)(const uint8_t);
using TUint8Void = uint8_t (*)();

class Transceiver
{
public:
  Transceiver(TVoidUint8 on_transmit, TUint8Void on_receive) : on_transmit_byte_{on_transmit}, on_receive_byte_{on_receive} {}

  void Transmit(const Payload &payload) const;
  Payload Receive(uint8_t expected_count = 1 + kCRCSize) const;

private:
  TVoidUint8 on_transmit_byte_;
  TUint8Void on_receive_byte_;
};