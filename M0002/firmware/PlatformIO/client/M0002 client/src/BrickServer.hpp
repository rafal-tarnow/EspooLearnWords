
#pragma once
#include <ESPAsyncTCP.h>
#include <Arduino.h>
#include <functional>
#include <string>
#include <utility>
#include <map>
#include "PseudoDNS.h"
#include "ProtocolStd.h"

class BrickServer
{
    using CallbackGet = std::function<void()>;
    using CallbackSaveNetworkSettings = std::function<void(const std::string &ssid, const std::string &pwd)>;
    using CallbackSaveBrickName = std::function<void(const std::string &brickName)>;

public:
    ~BrickServer();
    void begin(const std::string& brickName);
    template <typename T>
    void onGetNetworkSetting(T *obj, void (T::*method)())
    {
        callbackGetNetworkSettings = std::bind(method, obj);
    }
    template <typename T>
    void onGetBrickName(T *obj, void (T::*method)())
    {
        callbackGetBrickName = std::bind(method, obj);
    }
    template <typename T>
    void onSaveNetworkSetting(T *obj, void (T::*method)(const std::string &ssid, const std::string &pwd))
    {
        callbackSaveNetworkSettings = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
    }
    template <typename T>
    void onSaveBrickName(T *obj, void (T::*method)(const std::string &brickName))
    {
        callbackSaveBrickName = std::bind(method, obj, std::placeholders::_1);
    }
    void setBrickName(const std::string& brickName);
    void cmdSetBrickName(AsyncClient * client, const std::string& brickName);
    void cmdSetNetworkSettings(AsyncClient * client, const std::string& ssid, const std::string& pswd);
    virtual std::string getBrickType() const = 0;
    bool isSomeoneConnected();
    void update();

protected:
    void sendProtocolFrame(AsyncClient *client, const std::vector<uint8_t> &frame);

private:
    // server events
    void onHandleNewClient(void *arg, AsyncClient *client);
    // clients events
    void handleTcpError(void *arg, AsyncClient *client, int8_t error);
    void handleTcpData(void *arg, AsyncClient *client, void *data, size_t len);
    void handleTcpDisconnect(void *arg, AsyncClient *client);
    void handleTcpTimeOut(void *arg, AsyncClient *client, uint32_t time);

    void handleProtocolStdFrame(ProtocolStd *, std::deque<uint8_t> &frame);

private:
    CallbackGet callbackGetNetworkSettings;
    CallbackGet callbackGetBrickName;
    CallbackSaveNetworkSettings callbackSaveNetworkSettings;
    CallbackSaveBrickName callbackSaveBrickName;
    uint16_t PORT = 2883;
    AsyncServer *mServer;
    PseudoDNS pseudoDNS;
    std::map<AsyncClient*, ProtocolStd *> clients;
    std::map<ProtocolStd*, AsyncClient *> protocols;
};
