#include "osi_layers/session.h"

#include "utilities.h"

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
TString Session<>::Receive(const uint8_t from) const
{
    TString result{};
    static_cast<void>(from);
    return result;
}

template <>
LoginStatus Session<>::Login(const containers::static_string<> &user, const containers::static_string<> &pass)
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

template <>
void Session<>::Logout()
{
    cookie_ = {};
}
