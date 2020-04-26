#include "protocol.h"
#include "utilities.h"
#include <iostream>

template <>
CommunicationStatus UartHandshake<>::RetransmitWithAcknowledge(const Payload &payload)
{
    uint8_t retransmit_count = 2;
    CommunicationStatus result{CommunicationStatus::Unknown};
    for (uint8_t i = 0; i < retransmit_count; ++i)
    {
        io_.Transmit(payload);

        Payload response = io_.Receive();

        log_dump_payload(response, "response");

        if (response.size && static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge) == response.data[0])
        {
            log("Received negative response, retransmitting.");
            result = CommunicationStatus::NegativeAcknowledge;
        }
        else if (response.size && static_cast<uint8_t>(CommunicationStatus::Acknowledge) == response.data[0])
        {
            log("Received acknowledge response, everything OK.");
            result = CommunicationStatus::Acknowledge;
            break;
        }
    }
    return result;
}
