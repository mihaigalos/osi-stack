#include "osi_layers/datalink.h"
#include "utilities.h"
#include "crc.h"

template <>
CommunicationStatus Datalink<>::TransmitWithAcknowledge(const Payload &payload, uint8_t retransmit_count) const
{
    CommunicationStatus result{CommunicationStatus::Unknown};
    auto payload_with_crc = append_crc_to_payload(payload);

    log_dump_payload(payload_with_crc, "Datalink :: TransmitWithAcknowledge");
    for (uint8_t i = 0; i < retransmit_count && result != CommunicationStatus::Acknowledge; ++i)
    {
        io_.Transmit(payload_with_crc);
        Payload response = io_.Receive();

        log_dump_payload(response, "Datalink :: (response)");

        if (response.size && crc_match(response))
        {
            result = static_cast<CommunicationStatus>(response.data[0]);
            log("Received CommunicationStatus response: " + std::to_string(response.data[0]));
        }
        else
        {
            log("CRC mismatch!");
        }
    }
    return result;
}

template <>
Payload Datalink<>::ReceiveWithAcknowledge() const
{
    Payload received;
    CommunicationStatus status{CommunicationStatus::Unknown};

    for (uint8_t i = 0; i < kMaxRetransmitCount && status != CommunicationStatus::Acknowledge; ++i)
    {
        received = io_.Receive();
        log_dump_payload(received, " Datalink :: ReceiveWithAcknowledge :: received");

        status = crc_match(received) ? CommunicationStatus::Acknowledge : CommunicationStatus::NegativeAcknowledge;

        uint8_t data_response_[]{static_cast<uint8_t>(status)};
        io_.Transmit(append_crc_to_payload(Payload{data_response_, 1}));
    }

    return crc_match(received) ? received : Payload{};
}
