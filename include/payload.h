#pragma once

#include <cstdint>
#include "config.h"

struct Payload
{
    uint8_t size{0};
    uint8_t data[kPayloadMaxSize]{};

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

    const Payload &operator=(const Payload &rhs)
    {
        clone(rhs.data, rhs.size);
        return *this;
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
        for (uint8_t i = 0; i < kPayloadMaxSize; ++i)
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