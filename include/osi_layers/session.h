#pragma once
#include <cstdint>

#include "osi_layers/physical.h"

#include "config.h"
#include "transport.h"

enum class LoginStatus : uint8_t
{
    Unknown = 0x00,
    Error = 0x01,
    InvalidCredentials = 0x02,
    Success = 0x02,
};

template <typename TransportLayer = Transport<Network<Datalink<Physical, CRC>>>>
class Session
{
public:
    Session(TransportLayer &&transport, TString &&user, TString &&pass, uint8_t port) : transport_{std::forward<TransportLayer>(transport)}, user_{user}, pass_{pass}, port_{port}, cookie_{} {}

    CommunicationStatus Transmit(const uint8_t to, uint8_t *data, uint32_t total_size) const;
    CommunicationStatus Transmit(const uint8_t to, const char *data, uint32_t total_size) const;
    TString Receive(const uint8_t from_id, uint8_t port) const;

    LoginStatus Login(const TString &user, const TString &pass);
    void Logout();
    void deserializeUserPassword(TString &in, TString &user, TString &pass);
    bool IsLoggedIn() const { return cookie_ != decltype(cookie_){}; }

    virtual ~Session() = default;
    Session(const Session &other) = delete;
    Session(Session &&other) = delete;
    Session &operator=(const Session &other) = delete;
    Session &operator=(Session &&other) = delete;

private:
    TransportLayer transport_;
    TString user_;
    TString pass_;
    uint8_t port_;
    uint16_t cookie_;
};
