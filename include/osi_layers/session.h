#pragma once
#include <cstdint>

#include "osi_layers/physical.h"

#include "config.h"
#include "transport.h"

using TVoidCommunicationStatus = void (*)(CommunicationStatus);

enum class LoginStatus : uint8_t
{
    Unknown = 0x00,
    Error = 0x01,
    InvalidCredentials = 0x02,
    Success = 0x03,
};

void defaultOnCookieReceived(CommunicationStatus)
{
}

template <typename TransportLayer = Transport<Network<Datalink<Physical, CRC>>>>
class Session
{
public:
    Session(TransportLayer &&transport, TString &&user, TString &&pass, uint8_t port, TVoidCommunicationStatus onCookieReceived = defaultOnCookieReceived) : transport_{std::forward<TransportLayer>(transport)}, user_{user}, pass_{pass}, port_{port}, cookie_{}, onCookieReceived_{onCookieReceived} {}

#ifdef TESTING
    Session(TString &&user, TString &&pass, uint8_t port, TVoidCommunicationStatus onCookieReceived = defaultOnCookieReceived) : user_{user}, pass_{pass}, port_{port}, cookie_{}, onCookieReceived_{onCookieReceived}
    {
    }
#endif
    CommunicationStatus Transmit(const uint8_t to, TString &data) const
    {
        if (!IsLoggedIn())
        {
            TString credentials{serializeUserPassword()};
            auto response = transport_.Transmit(to, credentials.c_str(), credentials.size(), port_);
            if (response == CommunicationStatus::Acknowledge || response == CommunicationStatus::NoAcknowledgeRequired)
            {
                cookie_ = ReceiveCookie(to, port_);
                onCookieReceived_(response);
            }
        }
        if (IsLoggedIn())
        {
            serializeCookie(data);
            return TransmitWithCookie(to, data);
        }
        return CommunicationStatus::SessionCookieError;
    }

    TString Receive(const uint8_t from_id, uint8_t port) const
    {
        TString result{};

        auto received = transport_.Receive(from_id, port);
        if (IsLoggedIn())
        {
            result = received;
        }
        else
        {
            result = attemptLogin(received);

            serializeCookie(result);
            TransmitWithCookie(from_id, result);
        }

        return result;
    }

    LoginStatus Login(const TString &user, const TString &pass) const
    {
        LoginStatus result{};
        if (user_ == user && pass_ == pass)
        {
            cookie_ += 0xBEEF;
            result = LoginStatus::Success;
        }
        else
        {
            result = LoginStatus::InvalidCredentials;
        }
        return result;
    }

    void Logout()
    {
        cookie_ = {};
    }
    bool IsLoggedIn() const { return cookie_ != decltype(cookie_){}; }

    virtual ~Session() = default;
    Session(const Session &other) = delete;
    Session(Session &&other) = delete;
    Session &operator=(const Session &other) = delete;
    Session &operator=(Session &&other) = delete;

private:
    TString attemptLogin(TString &in) const
    {
        TString user, pass;
        deserializeUserPassword(in, user, pass);
        return loginStatusToString(Login(user, pass));
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

    TString loginStatusToString(LoginStatus status) const
    {
        TString result{};
        switch (status)
        {

        case LoginStatus::Error:
            result = "ER";
            break;
        case LoginStatus::InvalidCredentials:
            result = "IC";
            break;
        case LoginStatus::Success:
            result = "OK";
            break;
        default:
        case LoginStatus::Unknown:
            result = "KO";
            break;
        }
        return result;
    }

    bool isSuccess(TString &in) const
    {
        return (in[0] == 'O' && in[1] == 'K');
    }
    auto deserializeCookie(TString &in) const
    {
        decltype(cookie_) received_cookie{};
        if (isSuccess(in))
        {
            received_cookie = (in[3] << 8);
            received_cookie |= in[4];
        }
        return received_cookie;
    }

    void serializeCookie(TString &in) const
    {
        in += ' ';
        in += static_cast<char>(cookie_ >> 8);
        in += static_cast<char>(cookie_);
    }

    auto ReceiveCookie(const uint8_t from_id, uint8_t port) const
    {
        auto cookie = transport_.Receive(from_id, port);
        return deserializeCookie(cookie);
    }
    CommunicationStatus TransmitWithCookie(const uint8_t to, TString &data) const
    {
        return transport_.Transmit(to, data.c_str(), data.size(), port_);
    }

    TransportLayer transport_;
    TString user_;
    TString pass_;
    uint8_t port_;
    mutable uint16_t cookie_;
    TVoidCommunicationStatus onCookieReceived_;
};
