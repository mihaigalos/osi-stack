#include "osi_layers/datalink.h"
#include "utilities.h"
#include "crc.h"

template <>
CommunicationStatus Datalink<>::Transmit(const Payload &payload, uint8_t retransmit_count) const
{
    CommunicationStatus result{CommunicationStatus::Unknown};
    auto payload_with_crc = crc_.append_crc_to_payload(payload);

    log_dump_payload(payload_with_crc, "Datalink :: Transmit");
    for (uint8_t i = 0; i <= retransmit_count && result != CommunicationStatus::Acknowledge; ++i)
    {
        physical_.Transmit(payload_with_crc);
        Payload response = physical_.Receive();

        log_dump_payload(response, "Datalink :: (response)");

        if (response.size && crc_.crc_match(response))
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
Payload Datalink<>::Receive() const
{
    Payload received;
    CommunicationStatus status{CommunicationStatus::Unknown};

    for (uint8_t i = 0; i < kMaxRetransmitCount && status != CommunicationStatus::Acknowledge; ++i)
    {
        received = physical_.Receive();
        log_dump_payload(received, " Datalink :: Receive :: received");

        status = crc_.crc_match(received) ? CommunicationStatus::Acknowledge : CommunicationStatus::NegativeAcknowledge;

        uint8_t data_response_[]{static_cast<uint8_t>(status)};
        physical_.Transmit(crc_.append_crc_to_payload(Payload{data_response_, 1}));
    }

    return crc_.crc_match(received) ? received : Payload{};
}
