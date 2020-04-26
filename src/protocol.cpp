#include "protocol.h"
#include "utilities.h"
#include <iostream>

template <>
CommunicationStatus UartHandshake<>::RetransmitWithAcknowledge(const Payload &payload, uint8_t retransmit_count)
{
    CommunicationStatus result{CommunicationStatus::Unknown};
    for (uint8_t i = 0; i < retransmit_count; ++i)
    {
        io_.Transmit(payload);

        Payload response = io_.Receive();

        log_dump_payload(response, "response");

        if (response.size)
        {
            result = static_cast<CommunicationStatus>(response.data[0]);
            switch (result)
            {
            case CommunicationStatus::NegativeAcknowledge:
                log("Received negative response, retransmitting.");
                break;
            case CommunicationStatus::Acknowledge:
                log("Received acknowledge response, everything OK.");
                return CommunicationStatus::Acknowledge;
            default:
                break;
            }
        }
    }
    return result;
}
