#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include <containers/static_string.h>

#define private public // For testing purposes only

#include "osi_layers/physical.h"
#include "crc.h"
#include "osi_layers/session.h"

#include "utilities.h"
#include "unit/test_unit_base.h"

class MockTransport
{
public:
    TString Receive(const uint8_t from_id, uint8_t port) const
    {
        TString result{"myUsername myPassword"};
        static_cast<void>(from_id);
        static_cast<void>(port);
        return result;
    }

    CommunicationStatus Transmit(const uint8_t to, const uint8_t *data, const uint32_t total_size, const uint8_t port) const
    {
        CommunicationStatus result{CommunicationStatus::Unknown};
        static_cast<void>(to);
        static_cast<void>(data);
        static_cast<void>(total_size);
        static_cast<void>(port);
        return result;
    }
    CommunicationStatus Transmit(const uint8_t to, const char *data, const uint32_t total_size, const uint8_t port) const
    {
        CommunicationStatus result{CommunicationStatus::Unknown};
        static_cast<void>(to);
        static_cast<void>(data);
        static_cast<void>(total_size);
        static_cast<void>(port);
        return result;
    }
};

class Fixture : public UnitBase
{
public:
protected:
    Session<MockTransport> sut_{MockTransport{}, {"myUsername"}, {"myPassword"}, kPort};
};

TEST_F(Fixture, LoginSuccess_WhenTypical)
{
    TString expected = "Success";

    auto actual = sut_.Receive(kSourceId, kPort);

    ASSERT_EQ(actual, expected);
}
