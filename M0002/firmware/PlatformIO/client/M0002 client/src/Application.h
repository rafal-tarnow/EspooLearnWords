#pragma once
#include <led.h>
#include <Preferences.h>
#include "PseudoDNS.h"
#include "BrickClient.hpp"
#include "TcpServer.hpp"

template <typename T>
class Application
{
    enum AppState
    {
        CONFIG_STATE,
        RUN_STATE,
    };

public:
    void begin();

    // config button event handlers
    void handleButtonLongPress();
    void handleButtonClick();
    void handleButtonDoubleClick();

    // brick client events
    void handleGetBrickName(BrickClient *client);
    void handleBrickGetNetworkSettings(BrickClient *client);
    void handleBrickSaveNetworkSettings(BrickClient *client, const std::string &ssid, const std::string &pwd);
    void handleBrickSaveBrickName(BrickClient *client, const std::string &brickName);

    void handleWiFiStationModeConnected(const WiFiEventStationModeConnected &);
    void handleWiFiStationModeGotIp(const WiFiEventStationModeGotIP &);
    void wifiConnectToConfigNetwork();
    void wifiConnectToNetwork(const std::string &ssid, const std::string &pwd);
    void wifiDisconnectFromNetwork();
    void wifiCreateAccesPoint(const std::string &ssid, const std::string &pwd);
    void wifiDisconnectAccesPoint();
    void wifiDisconnectAll();

    std::string configReadBrickName();
    void configReadBrickName(std::string &brickName);
    void configReadNetworkSettings(std::string &ssid, std::string &pwd);
    void configSaveNetworkSettings(const std::string &ssid, const std::string &pwd);
    void configSaveBrickName(const std::string &brickName);

    // TcpServer events
    void handleTcpServerNewConnection();
    void update();
    virtual void loop() = 0;

protected:
    std::set<T *> clients;
    uint32_t disconnections;
    uint32_t connections;

private:
    // tcp AsyncServer events
    void handleNewTcpClient(void *arg, AsyncClient *client);
    void handleDisconnectedClient(BrickClient *client);   

    void handleNewTcpSocket(TcpSocket *socket);
    void setupNewBrickClientCallbacks(BrickClient *);


    void cleanup();

private:
#define AP_CONFIG_SSID "Aspoo Brick Config Network"
#define AP_CONFIG_PASSWORD "12345678"
    AppState appState = RUN_STATE;
    PseudoDNS pseudoDNS;
    uint16_t TCP_PORT = 2883;
    AsyncServer *asyncServer;
    TcpServer *tcpServer;
    Preferences prefs;
    WiFiEventHandler networkConnectedHandler;
    WiFiEventHandler stationModeGotIpHandler;

    std::set<T *> clientsToDelete;
};

#include "Application.tcc"