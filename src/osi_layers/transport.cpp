#include "osi_layers/transport.h"

#include "utilities.h"

inline uint16_t getSementsCount(const uint32_t size, const uint8_t payload_without_metadata)
{

    uint16_t full_segments{static_cast<uint16_t>(size / payload_without_metadata)};
    uint16_t partial_segments{static_cast<uint16_t>(size % payload_without_metadata)};
    uint16_t make_last_segment_be_zero{1};

    return {static_cast<uint16_t>(full_segments + (partial_segments != 0 ? 1 : 0) - make_last_segment_be_zero)};
}

template <>
CommunicationStatus Transport<>::Transmit(const uint8_t to, uint8_t *data, uint32_t size) const
{
    auto result{CommunicationStatus::Unknown};

    uint8_t payload_without_metadata{kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - kCRCSize - sizeof(TSegment)};

    TSegment segment{getSementsCount(size, payload_without_metadata)};

    for (uint32_t i = 0; i < size;)
    {
        uint8_t j{0};
        Payload payload{};
        for (j = 0; j <= (payload_without_metadata) && (i + j <= size); ++j)
        {
            payload.data[j] = data[i + j];
        }
        payload.size = j - 1;

        i += j - 1;

        payload.data[payload.size++] = static_cast<uint8_t>(segment);
        payload.data[payload.size++] = static_cast<uint8_t>(segment >> 8);
        log_dump_payload(payload, std::string{"Transport :: Transmit ["} + std::to_string(segment) + "]");
        --segment;

        result = network_.Transmit(to, payload);

        if (result != CommunicationStatus::Acknowledge)
        {
            break;
        }
    }

    return result;
}

template <>
containers::static_string<uint8_t, kSizeOfSegment> Transport<>::Receive(const uint8_t from) const
{
    containers::static_string<uint8_t, kSizeOfSegment> result{};
    TSegment segment{};
    TSegment watchdog{kSizeOfSegment};
    do
    {

        Payload received = network_.Receive(from);
        if (!received.size)
        {
            return result;
        }

        uint8_t segment_end{kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - kCRCSize};

        segment = received.data[segment_end - 1] << 8;
        segment |= received.data[segment_end - 2];

        for (uint8_t i = 0; i < segment_end - sizeof(TSegment); ++i)
        {
            const char character = static_cast<const char>(received.data[i]);
            result = result + character;
        }
    } while (segment && watchdog--);
    return result;
}
