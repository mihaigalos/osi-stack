#pragma once

#include <cstdint>

#include "osi_layers/physical.h"

#include "config.h"
#include "presentation.h"

template <typename PresentationLayer = Presentation<Session<Transport<Network<Datalink<Physical, CRC>>>>>, typename TEncryptDecrypt = SimpleTEA<>>
class Application
{
public:
    Application(PresentationLayer &&presentation, TString &&user, TString &&pass, uint8_t port) : presentation_{std::forward<PresentationLayer>(presentation)}, user_{user}, pass_{pass}, port_{port} {}
#ifdef TESTING
    Application(TString &&user, TString &&pass, uint8_t port) : user_{user}, pass_{pass}, port_{port}
    {
    }
#endif
    CommunicationStatus Transmit(const uint8_t to, TString &data) const
    {
        if (!presentation_.GetSession().IsSelfLoggedIn())
        {
            login(to, port_);
        }
        return presentation_.Transmit(to, port_, data);
    }

    TString Receive(const uint8_t from_id, uint8_t port) const
    {
        TString result = presentation_.Receive(from_id, port);

        if (!presentation_.GetSession().IsLoggedIn(from_id))
        {
            result = attemptLogin(result, from_id);
            if (static_cast<CommunicationStatus>(result[0]) != CommunicationStatus::InvalidCredentials)
            {
                log("Login successul. Transmitting cookie (encrypted).");
                result = presentation_.transmitEncryptCookie(from_id, port);
            }
        }
        return result;
    }

    CommunicationStatus Login(const TString &user, const TString &pass, const uint8_t from_id) const
    {
        CommunicationStatus result{};
        if (user_ == user && pass_ == pass)
        {
            result = presentation_.GetSession().Login(from_id);
        }
        else
        {
            result = CommunicationStatus::InvalidCredentials;
        }
        return result;
    }

    void Logout() const
    {
        presentation_.GetSession().Logout();
    }

    virtual ~Application() = default;
    Application(const Application &other) = delete;
    Application(Application &&other) = delete;
    Application &operator=(const Application &other) = delete;
    Application &operator=(Application &&other) = delete;

private:
    void login(const uint8_t from, uint8_t port) const
    {
        auto response = transmitCredentials(from);
        if (response == CommunicationStatus::Acknowledge || response == CommunicationStatus::NoAcknowledgeRequired)
        {
            presentation_.receiveDecryptCookie(from, port);
        }
    }
    CommunicationStatus transmitCredentials(const uint8_t to) const
    {
        TString credentials{serializeUserPassword()};
        return presentation_.Transmit(to, port_, credentials);
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
        credentials += const_cast<Application *>(this)->user_;
        credentials += ' ';
        credentials += const_cast<Application *>(this)->pass_;
        return credentials;
    }

    CommunicationStatus transmit(const uint8_t to, TString &data) const
    {
        return presentation_.Transmit(to, port_, data);
    }

    PresentationLayer presentation_{};
    TString user_{};
    TString pass_{};
    uint8_t port_{};
};
