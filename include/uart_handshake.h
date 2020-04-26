#pragma once

#include <cstdint>
#include "config.h"

constexpr uint8_t kPayloadMaxSize = 16;

struct Payload
{
  uint8_t data[kPayloadMaxSize];
  uint8_t size;

  Payload()
  {
    Reset();
  }

  Payload(const Payload &rhs)
  {
    clone(rhs.data, rhs.size);
  }

  Payload(const uint8_t *data, uint8_t size)
  {
    clone(data, size);
  }

  Payload(const char *data, uint8_t size)
  {
    clone(reinterpret_cast<const uint8_t *>(data), size);
  }

  void clone(const uint8_t *data, uint8_t size)
  {
    for (uint8_t i = 0; i < size; ++i)
    {
      this->data[i] = data[i];
    }
    this->size = size;
  }

  void Reset()
  {
    for (uint8_t i = 0; i < size; ++i)
    {
      data[i] = 0;
    }
    size = 0;
  }

#ifdef TESTING
  bool operator==(const Payload &rhs) const
  {
    if (size == rhs.size)
    {
      for (uint8_t i = 0; i < size; ++i)
      {
        if (data[i] != rhs.data[i])
        {

          return false;
        }
      }
      return true;
    }
    return false;
  }
  bool operator==(const uint8_t &rhs) const
  {
    bool result{false};
    if (size == 1 && data[0] == rhs)
    {
      result = true;
    }
    return result;
  }
#endif
};

using TVoidUint8 = void (*)(const uint8_t);
using TUint8Void = uint8_t (*)();

class UartIO
{
public:
  UartIO(TVoidUint8 on_transmit, TUint8Void on_receive) : on_transmit_byte_{on_transmit}, on_receive_byte_{on_receive} {}

  void Transmit(const Payload &payload);
  Payload Receive(uint8_t expected_count = 1 + kCRCSize);

private:
  TVoidUint8 on_transmit_byte_;
  TUint8Void on_receive_byte_;
};