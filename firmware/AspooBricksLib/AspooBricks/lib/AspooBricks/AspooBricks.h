#pragma once

#include <string>
#include <functional>
#include <vector>
#include <deque>
#include <PseudoDNS.h>
#include <StateMachine.h>
#include <KiKoTimer.h>


class KiKoBrick : public PseudoDNSEventListener
{
    typedef enum
    {
        INIT = 0,
        CONNECTING,
        CONNECTED,
        DISCONNECTED,
    } BrickState;
    using CallbackConnection = std::function<void(bool)>;

public:
    KiKoBrick();

    void onConnection(CallbackConnection callback)
    {
        callbackConnectionFunction = callback;
    }

    template <typename T>
    void onConnection(T *obj, void (T::*method)(bool))
    {
        callbackConnectionMethod = [obj, method](bool connected)
        {
            (obj->*method)(connected);
        };
    }

    void handleOnHostFound(const std::string &id, const std::string &type, const std::string &name, const std::string &ip) override;

    void handleEnterState(uint8_t state);
    void handleChangeState(uint8_t fromState, uint8_t toState);
    void handleExitState(uint8_t state);

    void handleProtocolFrame(std::deque<uint8_t> &frame);

    void setId(std::string);
    void begin();
    static void update();

protected:
    virtual void handleFrame(uint8_t functionCode, std::deque<uint8_t> &frame) = 0;

private:
    void loop();
    void onPingTimer();
    void sendProtocolFrame(const std::vector<uint8_t> &frame);

private:
    bool isConnected = false;
    std::string mId;
    static std::vector<KiKoBrick *> bricks;
    static PseudoDNS pseudoDNS;
    CallbackConnection callbackConnectionFunction;
    CallbackConnection callbackConnectionMethod;
    StateMachine stateMachine;
    WiFiClient client;
    ProtocolStd protocol;
    KiKoTimer pingTimer;
};

#include "AspooBrickT0002.h"
