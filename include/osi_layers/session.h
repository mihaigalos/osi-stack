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
    Session(TransportLayer &&transport, TString &&user, TString &&pass, uint8_t port) : transport_{std::forward<TransportLayer>(transport)}, user_{user}, pass_{pass}, port_{port} {}

#ifdef TESTING
    Session(TString &&user, TString &&pass, uint8_t port) : user_{user}, pass_{pass}, port_{port}
    {
    }
#endif
    CommunicationStatus Transmit(const uint8_t to, TString &data) const
    {
        if (!IsSelfLoggedIn())
        {
            login(to, port_);
        }
        if (IsSelfLoggedIn())
        {
            state_ = SessionState::TransmittingData;
            serializeCookie(data);
            return transmit(to, data);
        }

        log("Cannot login, false credentials.");

        data = {};
        data += static_cast<uint8_t>(CommunicationStatus::SessionCookieError);
        transmit(to, data);

        return CommunicationStatus::SessionCookieError;
    }

    TString Receive(const uint8_t from_id, uint8_t port) const
    {
        TString result{};

        auto received = transport_.Receive(from_id, port);
        if (IsLoggedIn(from_id))
        {
            result = received;
        }
        else
        {
            result = attemptLogin(received, from_id);

            serializeCookie(result, from_id);
            transmit(from_id, result);
        }

        return result;
    }

    CommunicationStatus Login(const TString &user, const TString &pass, const uint8_t from_id) const
    {
        CommunicationStatus result{};
        if (user_ == user && pass_ == pass)
        {
            clients_cookies_[from_id] += kCookieBaseValue;
            result = CommunicationStatus::Acknowledge;
        }
        else
        {
            result = CommunicationStatus::InvalidCredentials;
        }
        return result;
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

    virtual ~Session() = default;
    Session(const Session &other) = delete;
    Session(Session &&other) = delete;
    Session &operator=(const Session &other) = delete;
    Session &operator=(Session &&other) = delete;

private:
    void login(const uint8_t from, uint8_t port) const
    {
        auto response = transmitCredentials(from);
        state_ = SessionState::SentCredentials;
        if (response == CommunicationStatus::Acknowledge || response == CommunicationStatus::NoAcknowledgeRequired)
        {
            own_cookie_ = receiveCookie(from, port);
        }
    }
    CommunicationStatus transmitCredentials(const uint8_t to) const
    {
        TString credentials{serializeUserPassword()};
        return transport_.Transmit(to, credentials.c_str(), credentials.size(), port_);
    }

    TString attemptLogin(TString &in, const uint8_t from_id) const
    {
        TString user{}, pass{};
        TString result{};

        deserializeUserPassword(in, user, pass);
        result += static_cast<char>(Login(user, pass, from_id));

        return result;
    }

    void deserializeUserPassword(TString &in, TString &user, TString &pass) const
    {
        TString *out{&user};
        for (uint8_t i = 0; i < in.size(); ++i)
        {
            if (in[i] == ' ')
            {
                out->push_back('\0');
                out = &pass;
                continue;
            }
            out->push_back(in[i]);
        }
    }
    TString serializeUserPassword() const
    {
        TString credentials{};
        credentials += const_cast<Session *>(this)->user_;
        credentials += ' ';
        credentials += const_cast<Session *>(this)->pass_;
        return credentials;
    }

    bool isSuccess(TString &in) const
    {
        return (in[0] == static_cast<char>(CommunicationStatus::Acknowledge) && in[1] == ' ');
    }
    auto deserializeCookie(TString &in) const
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

    auto receiveCookie(const uint8_t from_id, uint8_t port) const
    {
        auto cookie = transport_.Receive(from_id, port);
        state_ = SessionState::ReceivedCookie;
        return deserializeCookie(cookie);
    }
    CommunicationStatus transmit(const uint8_t to, TString &data) const
    {
        return transport_.Transmit(to, data.c_str(), data.size(), port_);
    }

    TransportLayer transport_{};
    TString user_{};
    TString pass_{};
    uint8_t port_{};

    mutable TMapClientsCookies clients_cookies_{};
    mutable SessionState state_{};
    mutable uint16_t own_cookie_{};
};
