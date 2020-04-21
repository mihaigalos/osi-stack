#pragma once

#include <cstdint>

struct Payload
{
  const char *chars; // no smart pointers on avr architecture
  uint8_t size;

#ifdef TESTING
  bool operator==(const Payload &rhs) const
  {
    bool result{false};
    if (size == rhs.size)
    {
      for (uint8_t i = 0; i < size; ++i)
      {
        if (chars[i] != rhs.chars[i])
        {
          result = false;
        }
      }
      result = true;
    }
    return result;
  }
#endif
};

using TVoidPayload = void (*)(const Payload &);
using TPayloadVoid = Payload (*)();

class UartHanshake
{
public:
  UartHanshake(TVoidPayload on_transmit, TPayloadVoid on_receive) : on_transmit_{on_transmit}, on_receive_{on_receive} {}

  void Transmit(const Payload &payload);
  Payload Receive();

private:
  TVoidPayload on_transmit_;
  TPayloadVoid on_receive_;
};