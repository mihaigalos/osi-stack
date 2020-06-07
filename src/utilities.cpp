#include "utilities.h"

#include <iostream>
#include <iomanip>

#ifdef LOGGING
static uint64_t call_count{};

std::string payloadToString(const Payload &payload)
{
    std::string result;
    for (size_t i = 0; i < payload.size; ++i)
    {
        result += static_cast<char>(payload.data[i]);
    }
    return result;
}

void log_dump_payload(const Payload &payload, const std::string header)
{

    std::string current_stringified_payload = payloadToString(payload);

    std::cout << std::setfill(' ') << "---------" << std::left << std::setw(45) << header << "---------   " << std::right << std::setw(15);
    std::cout << "\033[1;32m"
              << "[" << std::dec << static_cast<int>(payload.size) << "] "
              << "\033[0m" << std::hex;
    for (uint8_t i = 0; i < payload.size; ++i)
    {

        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;

    if (++call_count % 4 == 0)
    {
        std::cout << std::endl;
    }
}

void log_dump_physical(const Payload &payload, const std::string header)
{

    std::cout << std::setfill(' ') << "---------" << std::left << std::setw(45) << header << "---------   " << std::right << std::setw(15);
    std::cout << "\033[1;32m"
              << "[" << std::dec << static_cast<int>(payload.size) << "] "
              << "\033[0m" << std::hex;
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        if (i == payload.size - kCRCSize - kSizeOfFromField - kSizeOfToField - sizeof(TSegment) - kSizeOfPort)
        {
            std::cout << "\033[1;31m";
        }
        if (i == payload.size - kCRCSize - kSizeOfFromField - kSizeOfToField - sizeof(TSegment))
        {
            std::cout << "\033[1;35m";
        }
        // if (i == payload.size - kCRCSize - kSizeOfFromField - kSizeOfToField - sizeof(TSegment))
        // {
        //     std::cout << "\033[1;33m";
        // }
        if (i == payload.size - kCRCSize - kSizeOfFromField - kSizeOfToField)
        {
            std::cout << "\033[1;33m";
        }
        if (i == payload.size - kCRCSize)
        {
            std::cout << "\033[1;36m";
        }
        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;

    if (++call_count % 4 == 0)
    {
        std::cout << std::endl;
    }
}

void log_dump_network(const Payload &payload, const std::string header)
{

    std::cout << std::setfill(' ') << "---------" << std::left << std::setw(45) << header << "---------   " << std::right << std::setw(15);
    std::cout << "\033[1;32m"
              << "[" << std::dec << static_cast<int>(payload.size) << "] "
              << "\033[0m" << std::hex;
    for (uint8_t i = 0; i < payload.size; ++i)
    {

        if (i == payload.size - kSizeOfFromField - kSizeOfToField - sizeof(TSegment) - kSizeOfPort)
        {
            std::cout << "\033[1;31m";
        }
        if (i == payload.size - kSizeOfFromField - kSizeOfToField - sizeof(TSegment))
        {
            std::cout << "\033[1;35m";
        }
        if (i == payload.size - kCRCSize)
        {
            std::cout << "\033[1;33m";
        }
        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;

    if (++call_count % 4 == 0)
    {
        std::cout << std::endl;
    }
}

void log_dump_transport(const Payload &payload, const std::string header)
{

    std::cout << std::setfill(' ') << "---------" << std::left << std::setw(45) << header << "---------   " << std::right << std::setw(15);
    std::cout << "\033[1;32m"
              << "[" << std::dec << static_cast<int>(payload.size) << "] "
              << "\033[0m" << std::hex;
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        if (i == payload.size - kSizeOfPort - sizeof(TSegment))
        {
            std::cout << "\033[1;31m";
        }
        if (i == payload.size - sizeof(TSegment))
        {
            std::cout << "\033[1;35m";
        }
        std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << "\033[0m";
    std::cout << std::dec << std::endl;

    if (++call_count % 4 == 0)
    {
        std::cout << std::endl;
    }
}
void log(std::string in)
{
    std::cout << in << std::endl;
}

#else

void log_dump_transport(const Payload &payload, const std::string header) {}
void log_dump_network(const Payload &payload, const std::string header) {}
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