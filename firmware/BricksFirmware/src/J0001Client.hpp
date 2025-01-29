#pragma once

#include <cstdint>

#include "BrickClient.hpp"
#include "Debug.hpp"

class J0001Client : public BrickClient
{
    using CallbackSetBool = std::function<void(J0001Client *, bool)>;
    using CallbackSetFloat = std::function<void(J0001Client *, float)>;
    using CallbackSetUint8 = std::function<void(J0001Client *, uint8_t)>;
    using CallbackEvent = std::function<void(J0001Client *)>;

public:
    J0001Client(AsyncClient *asyncClient);
    J0001Client(TcpSocket *tcpSocket);

    void cmdSetRun(bool run);
    void cmdSetDropletVolume(float dropletVolume);
    void cmdSetWindowDuration(uint8_t windowDuration);
    void cmdSetMeasureValues(uint64_t counter, float milliliters, float flowRate);
    void cmdSetRuntime(uint32_t seconds);
    void cmdSetMaxFlowDisplay(float maxFlowDisplay);

    
    static std::string type();
    void processProtocolStdFrame(std::deque<uint8_t> &frame) override;

    template <typename T>
    void onSetRunState(T *obj, void (T::*method)(J0001Client *, bool))
    {
        callbackSetRun = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void onSetDropletVolume(T *obj, void (T::*method)(J0001Client *, float))
    {
        callbackSetDropletVolume = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void onSetMaxFlowDisplay(T *obj, void(T::*method)(J0001Client*, float))
    {
        callbackSetMaxFlowDisplay = [obj, method](J0001Client* client, float maxFlowDisplay)
        {
            (obj->*method)(client, maxFlowDisplay);
        };
    }

    template <typename T>
    void onSetWindowDuration(T *obj, void (T::*method)(J0001Client *, uint8_t))
    {
        callbackSetWindowDuration = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void onResetMeasurement(T *obj, void(T::*method)(J0001Client *))
    {
        callbackResetMeasurement = [obj, method](J0001Client * client)
        {
            (obj->*method)(client);
        };
    }

private:
    CallbackSetBool callbackSetRun;
    CallbackSetFloat callbackSetDropletVolume;
    CallbackSetFloat callbackSetMaxFlowDisplay;
    CallbackSetUint8 callbackSetWindowDuration;
    CallbackEvent callbackResetMeasurement;
    DBG_COUNT("J0001Client");
};