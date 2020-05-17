#include "osi_layers/transport.h"

#include "utilities.h"

inline uint16_t getSementsCount(const uint32_t size, const uint8_t payload_without_metadata)
{

    uint16_t full_segments{static_cast<uint16_t>(size / payload_without_metadata)};
    uint16_t partial_segments{static_cast<uint16_t>(size % payload_without_metadata)};
    uint16_t make_last_segment_be_zero{1};

    return {static_cast<uint16_t>(full_segments + (partial_segments != 0 ? 1 : 0) - make_last_segment_be_zero)};
}

inline TString reconstructStringFromMap(TMap &buffer)
{
    TString result;
    uint8_t buffer_size = buffer.size();
    for (uint8_t i = 0; i < buffer_size; ++i)
    {
        result = result + buffer[i];
    }
    return result;
}

inline Payload constructPayloadFromData(uint8_t payload_without_metadata_size, uint32_t &i, uint32_t &total_size, uint8_t *data)
{
    uint8_t j{0};
    Payload payload{};
    for (j = 0; j <= (payload_without_metadata_size) && (i + j <= total_size); ++j)
    {
        payload.data[j] = data[i + j];
    }
    payload.size = j - 1;

    i += j - 1;

    return payload;
}

inline void serializeSegment(const TSegment &segment, Payload &payload)
{
    payload.data[payload.size++] = static_cast<uint8_t>(segment);
    payload.data[payload.size++] = static_cast<uint8_t>(segment >> 8);
}

inline TSegment deserializeSegment(const Payload &received, const uint8_t segment_end)
{
    TSegment segment = received.data[segment_end] << 8;
    segment |= received.data[segment_end - 1];
    return segment;
}

template <>
CommunicationStatus Transport<>::Transmit(const uint8_t to, uint8_t *data, uint32_t total_size) const
{
    auto result{CommunicationStatus::Unknown};

    uint8_t payload_without_metadata_size{kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - kCRCSize - sizeof(TSegment)};

    TSegment segment{getSementsCount(total_size, payload_without_metadata_size)};

    for (uint32_t i = 0; i < total_size;)
    {
        Payload payload = constructPayloadFromData(payload_without_metadata_size, i, total_size, data);
        serializeSegment(segment, payload);
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
TString Transport<>::Receive(const uint8_t from) const
{
    TString result{};
    TMap buffer{};
    TSegment segment{};
    TSegment watchdog{kSizeOfSegment};
    do
    {
        Payload received = network_.Receive(from);
        if (!received.size)
        {
            return result;
        }

        uint8_t segment_end = received.size - 1 - kSizeOfToField - kSizeOfFromField - kCRCSize;
        segment = deserializeSegment(received, segment_end);

        for (uint8_t i = 0; i <= segment_end - sizeof(TSegment); ++i)
        {
            buffer[i] = received.data[i];
        }

    } while (--segment && --watchdog);

    return reconstructStringFromMap(buffer);
}
