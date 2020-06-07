#include "utilities.h"

#include <iostream>
#include <iomanip>

#ifdef LOGGING

void log_dump_payload(const Payload &payload, const std::string header)
{

    std::cout << "---------" << header << "---------" << std::endl;
    std::cout << "\033[1;32m"
              << "[" << std::dec << static_cast<int>(payload.size) << "] "
              << "\033[0m" << std::hex;
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        if (i == payload.size - 2)
        {
            std::cout << "\033[1;36m";
        }
        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;
}

void log(std::string in)
{
    std::cout << in << std::endl;
}

#else

void log_dump_payload(const Payload &payload, const std::string header)
{
    static_cast<void>(payload);
    static_cast<void>(header);
}
void log(std::string in)
{
    static_cast<void>(in);
}

#endif