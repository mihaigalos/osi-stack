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
    Success = 0x03,
};

template <typename TransportLayer = Transport<Network<Datalink<Physical, CRC>>>>
class Session
{
public:
    Session(TransportLayer &&transport, TString &&user, TString &&pass, uint8_t port) : transport_{std::forward<TransportLayer>(transport)}, user_{user}, pass_{pass}, port_{port}, cookie_{} {}

#ifdef TESTING
    Session(TString &&user, TString &&pass, uint8_t port) : user_{user}, pass_{pass}, port_{port}, cookie_{}
    {
    }
#endif
    CommunicationStatus Transmit(const uint8_t to, TString &data) const
    {
        return transport_.Transmit(to, data.c_str(), data.size(), port_);
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
            appendCookie(result);
            Transmit(from_id, result);
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
    auto DeserializeCookie(TString &in) const
    {
        decltype(cookie_) received_cookie{};
        if (in[0] == 'O' && in[1] == 'K')
        {
            received_cookie = (in[3] << 8);
            received_cookie |= in[4];
        }
        return received_cookie;
    }

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

    void appendCookie(TString &in) const
    {
        in += ' ';
        in += static_cast<char>(cookie_ >> 8);
        in += static_cast<char>(cookie_);
    }

    TransportLayer transport_;
    TString user_;
    TString pass_;
    uint8_t port_;
    mutable uint16_t cookie_;
};
