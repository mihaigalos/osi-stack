#pragma once

#include <cstdint>

#include "osi_layers/physical.h"
#include "crc.h"

constexpr uint8_t payload_size_byte_count{1};
constexpr uint8_t data_acknowledge_[]{static_cast<uint8_t>(CommunicationStatus::Acknowledge)};
constexpr uint8_t data_negative_acknowledge_[]{static_cast<uint8_t>(CommunicationStatus::NegativeAcknowledge)};

class UnitBase : public ::testing::Test
{
public:
    virtual void SetUp() override
    {
        received_.Reset();
        transmitted_.Reset();

        payloadified_negative_acknowledge_ = Payload{data_negative_acknowledge_, 1};
        payloadified_acknowledge_ = Payload{data_acknowledge_, 1};

        payloadified_negative_acknowledge_ = append_crc_to_payload(payloadified_negative_acknowledge_);
        payloadified_acknowledge_ = append_crc_to_payload(payloadified_acknowledge_);

        payloadified_data_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
        payloadified_data_with_crc_ = Payload{data_.c_str(), static_cast<uint8_t>(data_.length())};
        payloadified_data_with_crc_ = append_crc_to_payload(payloadified_data_with_crc_);
        received_.size = 0;
    }
    virtual void TearDown() override {}

protected:
    static std::string data_;
    static Payload payloadified_negative_acknowledge_;
    static Payload payloadified_acknowledge_;
    static Payload payloadified_data_;
    static Payload payloadified_data_with_crc_;
    static Payload received_, transmitted_;
    static std::map<uint8_t, uint8_t> lookup_map_;
};

Payload UnitBase::received_;
Payload UnitBase::transmitted_;

std::string UnitBase::data_{"abcd"};
Payload UnitBase::payloadified_negative_acknowledge_;
Payload UnitBase::payloadified_acknowledge_;
Payload UnitBase::payloadified_data_;
Payload UnitBase::payloadified_data_with_crc_;

std::map<uint8_t, uint8_t> UnitBase::lookup_map_;
