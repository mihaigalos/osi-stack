#include "utilities.h"

#include <iostream>
#include <iomanip>

#ifdef LOGGING

void printHeader(const std::string header)
{
    std::cout << std::setfill(' ') << "           " << std::left << std::setw(35) << header << "           " << std::right << std::setw(15);
}

void printSize(uint32_t size)
{
    std::cout << "\033[1;32m"
              << "[" << std::dec << std::setfill(' ') << std::setw(2) << static_cast<int>(size) << "] "
              << "\033[0m" << std::hex;
}

void log_dump_physical(const Payload &payload, const std::string header)
{
    printHeader(header);
    printSize(payload.size);
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        if (i == payload.size - kCRCSize)
        {
            std::cout << "\033[1;36m";
        }
        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;
}
void log_dump_datalink(const Payload &payload, const std::string header)
{
    log_dump_physical(payload, header);
}
void log_dump_network(const Payload &payload, const std::string header)
{

    printHeader(header);
    printSize(payload.size);
    for (uint8_t i = 0; i < payload.size; ++i)
    {

        if (i == payload.size - kCRCSize)
        {
            std::cout << "\033[1;33m";
        }
        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;
}

void log_dump_transport(const Payload &payload, const std::string header)
{
    printHeader(header);
    printSize(payload.size);
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        if (i == payload.size - kSizeOfPort - sizeof(TSegment))
        {
            std::cout << "\033[1;31m";
        }
        else if (i == payload.size - sizeof(TSegment))
        {
            std::cout << "\033[1;35m";
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

void log_dump_transport(const Payload &payload, const std::string header) {}
void log_dump_network(const Payload &payload, const std::string header) {}
void log_dump_datalink(const Payload &payload, const std::string header) {}
void log_dump_physical(const Payload &payload, const std::string header) {}
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