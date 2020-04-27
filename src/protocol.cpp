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

template <>
Payload UartHandshake<>::ReceiveWithAcknowledge(uint8_t expected_count) const
{
    static_cast<void>(expected_count);
    Payload result;
    CommunicationStatus status{CommunicationStatus::Unknown};
    for (uint8_t i = 0; i < kMaxRetransmitCount && status != CommunicationStatus::Acknowledge; ++i)
    {
    }

    return result;
}
