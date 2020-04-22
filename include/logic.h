#pragma once

#include <cstdint>

using TVoidUint8 = void (*)(const uint8_t &);
using TUint8Void = uint8_t (*)();

class UartHanshake
{
public:
  UartHanshake(TVoidUint8 on_transmit, TUint8Void on_receive) : on_transmit_byte_{on_transmit}, on_receive_byte_{on_receive} {}

  void Transmit(const uint8_t &payload);
  uint8_t Receive();

private:
  TVoidUint8 on_transmit_byte_;
  TUint8Void on_receive_byte_;
};