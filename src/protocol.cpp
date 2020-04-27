#include "protocol.h"
#include "utilities.h"
#include <iostream>

template <>
CommunicationStatus UartHandshake<>::TransmitWithAcknowledge(const Payload &payload, uint8_t retransmit_count) const
{
    CommunicationStatus result{CommunicationStatus::Unknown};
    for (uint8_t i = 0; i < retransmit_count && result != CommunicationStatus::Acknowledge; ++i)
    {
        io_.Transmit(payload);
        Payload response = io_.Receive();

        log_dump_payload(response, "response");

        if (response.size)
        {
            result = static_cast<CommunicationStatus>(response.data[0]);
            log("Received CommunicationStatus response: " + std::to_string(response.data[0]));
        }
    }
    return result;
}
