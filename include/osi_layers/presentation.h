#pragma once

#include <cstdint>

#include "osi_layers/physical.h"

#include "config.h"
#include "session.h"
#include "simple_tea.h"

bool isOddLength(const TString &in)
{
    return in.size() & 1;
}

template <typename SessionLayer = Session<Transport<Network<Datalink<Physical, CRC>>>>, typename TEncryptDecrypt = SimpleTEA<>>
class Presentation
{
public:
    Presentation(SessionLayer &&session, uint8_t encryptionRounds) : session_{std::forward<SessionLayer>(session)}, encryptionRounds_{encryptionRounds} {}
    Presentation(Presentation &&other) : session_{std::forward<SessionLayer>(other.session_)}, encdec_{std::forward<TEncryptDecrypt>(other.encdec_)}, encryptionRounds_{other.encryptionRounds_} {}
#ifdef TESTING
    Presentation()
    {
    }
#endif

    virtual__ CommunicationStatus Transmit(const uint8_t to, const uint8_t port, TString &data) const
    {
        if (isOddLength(data))
        {
            data += '\0';
        }
        encdec_.encrypt(encryptionRounds_, kEncryptionKey, reinterpret_cast<uint8_t *>(const_cast<char *>(data.c_str())), data.size());

        return session_.Transmit(to, port, data);
    }

    virtual__ TString Receive(const uint8_t from_id, const uint8_t port) const
    {
        TEncryptedString result = session_.Receive(from_id, port).c_str();
        encdec_.decrypt(encryptionRounds_, kEncryptionKey, reinterpret_cast<uint8_t *>(const_cast<char *>(result.c_str())), result.size());
        return {result.c_str()};
    }

    virtual__ const SessionLayer &GetSession() const { return session_; }

    void receiveDecryptCookie(const uint8_t from_id, const uint8_t port) const
    {
        auto cookie = Receive(from_id, port);
        session_.SetCookie(session_.deserializeCookie(cookie), kSelf);
    }

    TString transmitEncryptCookie(const uint8_t to, const uint8_t port) const
    {
        TString data, result{};

        data += static_cast<char>(CommunicationStatus::Acknowledge);
        session_.serializeCookie(data, to);
        result = data;

        Transmit(to, port, data);
        return result;
    }

    virtual ~Presentation() = default;
    Presentation(const Presentation &other) = delete;

    Presentation &operator=(const Presentation &other) = delete;
    Presentation &operator=(Presentation &&other) = delete;

private:
    SessionLayer session_{};
    TEncryptDecrypt encdec_{};
    uint8_t encryptionRounds_{1};
};