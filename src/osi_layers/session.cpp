#include "osi_layers/session.h"

#include "utilities.h"

TString loginStatusToString(LoginStatus status)
{
    TString result{};
    switch (status)
    {

    case LoginStatus::Error:
        result = "Error";
        break;
    case LoginStatus::InvalidCredentials:
        result = "InvalidCredentials";
        break;
    case LoginStatus::Success:
        result = "Success";
        break;
    default:
    case LoginStatus::Unknown:
        result = "Unknown";
        break;
    }
    return result;
}

template <>
void Session<>::deserializeUserPassword(TString &in, TString &user, TString &pass) const
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

template <>
TString Session<>::attemptLogin(TString &in) const
{
    TString user, pass;
    deserializeUserPassword(in, user, pass);
    return loginStatusToString(Login(user, pass));
}

template <>
CommunicationStatus Session<>::Transmit(const uint8_t to, uint8_t *data, uint32_t total_size) const
{
    auto result{CommunicationStatus::Unknown};
    static_cast<void>(to);
    static_cast<void>(data);
    static_cast<void>(total_size);
    return result;
}

template <>
CommunicationStatus Session<>::Transmit(const uint8_t to, const char *data, uint32_t total_size) const
{
    return Transmit(to, reinterpret_cast<uint8_t *>(const_cast<char *>(data)), total_size);
}

template <>
TString Session<>::Receive(const uint8_t from_id, uint8_t port) const
{
    TString result{};

    auto received = transport_.Receive(from_id, port);
    if (IsLoggedIn())
    {
        result = received;
    }
    else
    {
        result = attemptLogin(result);
    }

    return result;
}

template <>
void Session<>::Logout()
{
    cookie_ = {};
}
