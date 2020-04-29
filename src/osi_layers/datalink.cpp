#include "osi_layers/datalink.h"
#include "utilities.h"
#include "crc.h"
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
    Payload received;
    CommunicationStatus status{CommunicationStatus::Unknown};

    for (uint8_t i = 0; i < kMaxRetransmitCount && status != CommunicationStatus::Acknowledge; ++i)
    {
        received = io_.Receive(expected_count);
        log_dump_payload(received, "received");

        status = crc_match(received) ? CommunicationStatus::Acknowledge : CommunicationStatus::NegativeAcknowledge;

        uint8_t data_response_[]{static_cast<uint8_t>(status)};
        io_.Transmit(Payload{data_response_, 1});
    }

    return received;
}
