#include "utilities.h"

#ifdef TESTING

void log_dump_payload(const Payload &payload, const std::string header)
{

    std::cout << "---------" << header << "---------" << std::endl;
    for (uint8_t i = 0; i < payload.size; ++i)
    {
        std::cout << std::dec << static_cast<int>(payload.data[i]) << " ";
    }
    std::cout << std::endl;
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