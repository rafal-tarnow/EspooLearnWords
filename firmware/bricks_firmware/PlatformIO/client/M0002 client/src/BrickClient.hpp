#pragma once
#include <functional>
#include <string>
#include <deque>
#include <ESPAsyncTCP.h>
#include <Arduino.h>
#include <functional>
#include <string>
#include <utility>
#include <unordered_set>
#include "TcpSocket.hpp"
#include "dtime.h"
#include "ProtocolStd.h"
#include "Debug.hpp"
#include "config.h"

class BrickClient
{
    friend class BrickServer;
    using CallbackGet = std::function<void(BrickClient *)>;
    using CallbackSaveNetworkSettings = std::function<void(BrickClient *, const std::string &ssid, const std::string &pwd)>;
    using CallbackSaveBrickName = std::function<void(BrickClient *, const std::string &brickName)>;

public:
    BrickClient(AsyncClient *asyncClient);
    BrickClient(TcpSocket *socket);
    ~BrickClient();

    template <typename T>
    void onGetInfo(T *obj, void (T::*method)(BrickClient *))
    {
        callbackGetInfo = [obj, method](BrickClient *client)
        {
            (obj->*method)(client);
        };
    }

    template <typename T>
    void onGetId(T *obj, void (T::*method)(BrickClient *))
    {
        callbackGetId = [obj, method](BrickClient *client)
        {
            (obj->*method)(client);
        };
    }
    template <typename T>
    void onGetType(T *obj, void (T::*method)(BrickClient *))
    {
        callbackGetType = [obj, method](BrickClient *client)
        {
            (obj->*method)(client);
        };
    }
    template <typename T>
    void onGetNetworkSetting(T *obj, void (T::*method)(BrickClient *))
    {
        callbackGetNetworkSettings = std::bind(method, obj, std::placeholders::_1);
    }
    template <typename T>
    void onGetBrickName(T *obj, void (T::*method)(BrickClient *))
    {
        callbackGetBrickName = std::bind(method, obj, std::placeholders::_1);
    }

    template <typename T>
    void onSaveNetworkSetting(T *obj, void (T::*method)(BrickClient *, const std::string &ssid, const std::string &pwd))
    {
        callbackSaveNetworkSettings = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }
    template <typename T>
    void onSaveBrickName(T *obj, void (T::*method)(BrickClient *, const std::string &brickName))
    {
        callbackSaveBrickName = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void onTcpDisconnected(T *obj, void (T::*method)(BrickClient *))
    {
        callbackTcpDisconnected = [obj, method](BrickClient *client)
        {
            (obj->*method)(client);
        };
    }

    void close();

    void cmdSetInfo(const std::string &id, const std::string &brickType, const std::string &brickName, const std::string &ssid, const std::string &pswd);
    void cmdSetId(const std::string &id);
    void cmdSetType(const std::string &brickType);
    void cmdSetName(const std::string &brickName);
    void cmdSetNetworkSettings(const std::string &ssid, const std::string &pswd);

    static void printDbg();
    size_t space();
    static bool isAnyWaiting();
    static void tryFlushBuffers();
    virtual void processProtocolStdFrame(std::deque<uint8_t> &frame) = 0;

protected:
    void sendProtocolFrame(const std::vector<uint8_t> &frame);

private:
    // tcp AsyncClient events
    void handleTcpData(void *arg, AsyncClient *client, void *data, size_t len);
    void handleTcpError(void *arg, AsyncClient *client, int8_t error);
    void handleTcpDisconnect(void *arg, AsyncClient *client);
    void handleTcpTimeOut(void *arg, AsyncClient *client, uint32_t time);

    // tcp TcpSocket events
    void handleTcpSocketData(const uint8_t *buffer, uint16_t len);
    void handleTcpSocketDisconnect();
    void handleTcpSocketError(const std::string &);

    // protocol events
    void handleProtocolStdFrame(std::deque<uint8_t> &frame);
    static bool isWaiting(BrickClient *);
    bool tryFlushBuffer();

private:
    static std::unordered_set<BrickClient *> waitingClients;
    static uint32_t instancesCreated;
    static uint32_t disconnections;
    static uint32_t errorsCount;
    static uint32_t timeouts;
    static uint64_t bytesRx;
    static std::vector<std::string> errors;
    CallbackGet callbackGetInfo;
    CallbackGet callbackGetId;
    CallbackGet callbackGetType;
    CallbackGet callbackGetBrickName;
    CallbackGet callbackGetNetworkSettings;
    CallbackSaveNetworkSettings callbackSaveNetworkSettings;
    CallbackSaveBrickName callbackSaveBrickName;
    CallbackGet callbackTcpDisconnected;
    AsyncClient *asyncClient = nullptr;
    TcpSocket *tcpSocket = nullptr;
    ProtocolStd protocol;
    std::vector<uint8_t> bufferToSend;
    DBG_COUNT("BrickClient");
};