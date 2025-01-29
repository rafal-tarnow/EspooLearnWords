#pragma once
#include "BrickClient.hpp"
#include "Debug.hpp"

//single relay 

class M0004 : public BrickClient
{
        using CallbackSetRelay = std::function<void(M0004 *, bool)>;
public:
    M0004(AsyncClient *asyncClient);
    M0004(TcpSocket *tcpSocket);

    template <typename T>
    void onSetRelayState(T *obj, void (T::*method)(M0004 *, bool))
    {
        callbackSetRelay = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
    }

    static std::string type();
    void processProtocolStdFrame(std::deque<uint8_t> &frame) override;

private:
    CallbackSetRelay callbackSetRelay;
    DBG_COUNT("K0004");
};