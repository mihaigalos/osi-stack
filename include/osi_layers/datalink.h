#pragma once

#include <cstdint>
#include "osi_layers/physical.h"
#include "config.h"

#include "crc.h"
#include "utilities.h"
#include <utility>

enum class CommunicationStatus : uint8_t
{
    Unknown = 0x00,
    Error = 0x01,
    NoAcknowledgeRequired = 0x02,
    CRCMismatch = 0x03,
    Acknowledge = 0x06,
    NegativeAcknowledge = 0x15,
};

template <typename PhysicalLayer = Physical, typename CRCFunctions = CRC>
class Datalink
{
public:
    Datalink(PhysicalLayer &&physical) : physical_{std::forward<PhysicalLayer>(physical)} {}
    Datalink(Datalink &&other) : physical_{std::forward<PhysicalLayer>(other.physical_)}, retransmit_count_{other.retransmit_count_} {}
    CommunicationStatus Transmit(const Payload &payload) const
    {
        CommunicationStatus result{CommunicationStatus::Unknown};
        auto payload_with_crc = crc_.append_crc_to_payload(payload);

        log_dump_datalink(payload_with_crc, "Datalink :: Transmit");
        for (uint8_t i = 0; i <= retransmit_count_ && result != CommunicationStatus::Acknowledge; ++i)
        {
            physical_.Transmit(payload_with_crc);
            Payload response = GetTransmitResponse();

            log_dump_datalink(response, "Datalink :: (response)");

            if (response.size)
            {
                if (crc_.crc_match(response))
                {
                    result = static_cast<CommunicationStatus>(response.data[0]);
                    log("Received CommunicationStatus response: " + std::to_string(response.data[0]));
                }
                else
                {
                    result = CommunicationStatus::CRCMismatch;
                    log("CRC mismatch!");
                }
            }
            else
            {
                result = CommunicationStatus::NoAcknowledgeRequired;
            }
        }
        return result;
    }
    Payload Receive() const
    {
        Payload received;
        CommunicationStatus status{CommunicationStatus::Unknown};

        for (uint8_t i = 0; i <= retransmit_count_ && status != CommunicationStatus::Acknowledge; ++i)
        {
            received = physical_.Receive();
            log_dump_datalink(received, "Datalink :: Receive :: received");

            status = crc_.crc_match(received) ? CommunicationStatus::Acknowledge : CommunicationStatus::NegativeAcknowledge;

            uint8_t data_response_[]{static_cast<uint8_t>(status)};
            physical_.Transmit(crc_.append_crc_to_payload(Payload{data_response_, 1}));
        }

        return crc_.crc_match(received) ? received : Payload{};
    }

#ifdef TESTING
    Datalink()
    {
    }
#endif

    virtual ~Datalink() = default;
    Datalink &operator=(const Datalink &other) = delete;
    Datalink &operator=(Datalink &&other) = delete;

private:
    PhysicalLayer physical_;
    CRCFunctions crc_;
    uint8_t retransmit_count_{kMaxRetransmitCount};

    Payload GetTransmitResponse() const
    {
        if (retransmit_count_)
        {
            return physical_.Receive();
        }

        return {};
    }
};