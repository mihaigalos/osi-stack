#include "osi_layers/transport.h"

#include "utilities.h"

inline uint16_t getSegmentsCount(const uint32_t size)
{

    uint16_t full_segments{static_cast<uint16_t>(size / kTransportPayloadSize)};
    uint16_t partial_segments{static_cast<uint16_t>(size % kTransportPayloadSize)};
    uint16_t make_last_segment_be_zero{1};

    return {static_cast<uint16_t>(full_segments + (partial_segments != 0 ? 1 : 0) - make_last_segment_be_zero)};
}

inline TString reconstructStringFromMap(TMapSequencePayload &buffer)
{
    TString result;
    uint8_t buffer_size = buffer.size();
    for (uint8_t i = buffer_size - 1;; --i)
    {
        Payload payload = buffer[i];
        for (uint8_t j = 0; j < payload.size; ++j)
        {
            result = result + payload.data[j];
        }

        if (i == 0)
        {
            break;
        }
    }
    return result;
}

inline Payload constructPayloadFromData(const uint32_t &total_size, const uint8_t *data, uint32_t &serialized_bytes)
{
    uint8_t i{0};
    Payload payload{};
    for (i = 0; i <= (kTransportPayloadSize) && (serialized_bytes + i <= total_size); ++i)
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

inline void serializePort(const uint8_t port, Payload &payload)
{
    payload.data[payload.size++] = port;
}

inline TSegment deserializeSegment(const Payload &received, const uint8_t segment_end)
{
    TSegment segment = received.data[segment_end] << 8;
    segment |= received.data[segment_end - 1];
    return segment;
}

inline uint8_t deserializePort(const Payload &received, const uint8_t port_end)
{
    return received.data[port_end];
}

inline Payload serializeData(const uint32_t &total_size, const uint8_t *data, const TSegment &segment, const uint8_t port, uint32_t &serialized_bytes)
{
    Payload payload = constructPayloadFromData(total_size, data, serialized_bytes);
    serializePort(port, payload);
    serializeSegment(segment, payload);
    return payload;
}

inline void deserializeData(const Payload &received, TSegment &segment, TMapPortSequencePayload &port_sequece_payload, uint8_t &port)
{
    uint8_t segment_end = received.size - 1 - kSizeOfToField - kSizeOfFromField - kCRCSize;
    uint8_t port_begining = segment_end - sizeof(TSegment);
    // constexpr uint8_t kTransportPayloadSize{kPayloadMaxSize - kSizeOfToField - kSizeOfFromField - sizeof(TSegment) - kSizeOfPort - kCRCSize};
    port = deserializePort(received, port_begining);
    segment = deserializeSegment(received, segment_end);

    Payload deserialized{};
    for (uint8_t i = 0; i < port_begining; ++i)
    {
        deserialized.data[deserialized.size++] = received.data[i];
    }
    port_sequece_payload[port][segment] = deserialized;
}

template <>
CommunicationStatus Transport<>::Transmit(const uint8_t to, const uint8_t *data, const uint32_t total_size, const uint8_t port) const
{
    auto result{CommunicationStatus::Unknown};

    TSegment segment{getSegmentsCount(total_size)};

    for (uint32_t serialized_bytes = 0; serialized_bytes < total_size;)
    {
        Payload payload = serializeData(total_size, data, segment, port, serialized_bytes);
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
CommunicationStatus Transport<>::Transmit(const uint8_t to, const char *data, const uint32_t total_size, const uint8_t port) const
{
    return Transmit(to, reinterpret_cast<uint8_t *>(const_cast<char *>(data)), total_size, port);
}

template <>
TString Transport<>::Receive(const uint8_t from_id, uint8_t port) const
{
    TString result{};
    TMapPortSequencePayload port_sequece_payload{};
    TSegment segment{};
    TSegment watchdog{kMaximumSegments};
    do
    {
        Payload received = network_.Receive(from_id);
        if (!received.size)
        {
            return result;
        }

        deserializeData(received, segment, port_sequece_payload, port);

    } while (segment && --watchdog);

    return reconstructStringFromMap(port_sequece_payload[port]);
}
