#pragma once
#include <cstdint>

#include "osi_layers/physical.h"

#include "config.h"
#include "transport.h"

enum class SessionState
{
    Unknown,
    SentCredentials,
    ReceivedCookie,
    TransmittingData
};

constexpr uint8_t kSelf{0xFA};

template <typename TransportLayer = Transport<Network<Datalink<Physical, CRC>>>>
class Session
{
public:
    Session(TransportLayer &&transport) : transport_{std::forward<TransportLayer>(transport)} {}
    Session(Session &&other) : transport_{std::forward<TransportLayer>(other.transport_)} {}

#ifdef TESTING
    Session(TString &&user, TString &&pass, uint8_t port) : user_{user}, pass_{pass}, port_{port}
    {
    }
#endif
    CommunicationStatus Transmit(const uint8_t to, TString &data) const
    {
        return transport_.Transmit(to, data.c_str(), data.size(), port_);
    }

    TString Receive(const uint8_t from_id, uint8_t port) const
    {
        return transport_.Receive(from_id, port);
    }

    CommunicationStatus Login(const uint8_t from_id) const
    {
        clients_cookies_[from_id] += kCookieBaseValue;
        return CommunicationStatus::Acknowledge;
    }

    void Logout()
    {
        own_cookie_ = decltype(own_cookie_){};
    }
    bool IsSelfLoggedIn() const { return own_cookie_ != decltype(own_cookie_){}; }
    bool IsLoggedIn(const uint8_t from_id) const
    {
        return clients_cookies_[from_id] != decltype(own_cookie_){};
    }

    void SetCookie(uint16_t cookie, uint8_t from_id = kSelf)
    {
        if (from_id == kSelf)
        {
            own_cookie_ = cookie;
        }
        else
        {
            clients_cookies_[from_id] = cookie;
        }
    }

    virtual ~Session() = default;
    Session(const Session &other) = delete;

    Session &operator=(const Session &other) = delete;
    Session &operator=(Session &&other) = delete;

private:
    bool isSuccess(TString &in) const
    {
        return (in[0] == static_cast<char>(CommunicationStatus::Acknowledge) && in[1] == ' ');
    }
    auto serializeCookie(TString &in) const
    {
        decltype(own_cookie_) received_cookie{};
        if (isSuccess(in))
        {
            received_cookie = (in[2] << 8);
            received_cookie |= in[3];
        }
        return received_cookie;
    }

    void serializeCookie(TString &in, uint8_t from_id = kSelf) const
    {
        in += ' ';
        if (from_id == kSelf)
        {
            in += static_cast<char>(own_cookie_ >> 8);
            in += static_cast<char>(own_cookie_);
        }
        else
        {
            in += static_cast<char>(clients_cookies_[from_id] >> 8);
            in += static_cast<char>(clients_cookies_[from_id]);
        }
    }

    TransportLayer transport_{};

    mutable TMapClientsCookies clients_cookies_{};
    mutable SessionState state_{};
    mutable uint16_t own_cookie_{};
};
