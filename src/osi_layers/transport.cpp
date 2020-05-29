#include "osi_layers/transport.h"

#include "utilities.h"

inline uint16_t getSegmentsCount(const uint32_t size)
{

    uint16_t full_segments{static_cast<uint16_t>(size / transport_payload_size)};
    uint16_t partial_segments{static_cast<uint16_t>(size % transport_payload_size)};
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

inline Payload constructPayloadFromData(const uint32_t &total_size, const uint8_t *data, uint32_t &serialized_bytes)
{
    uint8_t i{0};
    Payload payload{};
    for (i = 0; i <= (transport_payload_size) && (serialized_bytes + i <= total_size); ++i)
    {
        payload.data[i] = data[serialized_bytes + i];
    }
    payload.size = i - 1;

    serialized_bytes += i - 1;

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

inline Payload serializeData(const uint32_t &total_size, const uint8_t *data, const TSegment &segment, uint32_t &serialized_bytes)
{
    Payload payload = constructPayloadFromData(total_size, data, serialized_bytes);
    serializeSegment(segment, payload);
    return payload;
}

inline void deserializeData(const Payload &received, TSegment &segment, TMap &buffer)
{
    uint8_t segment_end = received.size - 1 - kSizeOfToField - kSizeOfFromField - kCRCSize;
    segment = deserializeSegment(received, segment_end);

    for (uint8_t i = 0; i <= segment_end - sizeof(TSegment); ++i)
    {
        buffer[i] = received.data[i];
    }
}

template <>
CommunicationStatus Transport<>::Transmit(const uint8_t to, uint8_t *data, uint32_t total_size) const
{
    auto result{CommunicationStatus::Unknown};

    TSegment segment{getSegmentsCount(total_size)};

    for (uint32_t serialized_bytes = 0; serialized_bytes < total_size;)
    {
        Payload payload = serializeData(total_size, data, segment, serialized_bytes);
        log_dump_payload(payload, std::string{"Transport :: Transmit ["} + std::to_string(segment) + "]");
        --segment;

        result = network_.Transmit(to, payload);

        if (result != CommunicationStatus::Acknowledge && result != CommunicationStatus::NoAcknowledgeRequired)
        {
            break;
        }
    }

    return result;
}

template <>
CommunicationStatus Transport<>::Transmit(const uint8_t to, const char *data, uint32_t total_size) const
{
    return Transmit(to, reinterpret_cast<uint8_t *>(const_cast<char *>(data)), total_size);
}

template <>
TString Transport<>::Receive(const uint8_t from) const
{
    TString result{};
    TMap buffer{};
    TSegment segment{};
    TSegment watchdog{kMaximumSegments};
    do
    {
        Payload received = network_.Receive(from);
        if (!received.size)
        {
            return result;
        }

        deserializeData(received, segment, buffer);

    } while (segment && --watchdog);

    return reconstructStringFromMap(buffer);
}
