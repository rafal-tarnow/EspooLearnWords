#pragma once
#include <led.h>
#include <Preferences.h>
#include "BrickServer.hpp"
#include "BrickClient.hpp"

class Application
{
    enum AppState
    {
        CONFIG_STATE,
        RUN_STATE,
    };

public:
    void begin();
    void handleButtonLongPress();
    void handleButtonClick();
    void handleButtonDoubleClick();

    // brick server events
    BrickClient *handleBrickClientCreate(AsyncClient *);
    void handleBrickClientDelete(BrickClient *);

    // brick client events
    void handleGetBrickName(BrickClient *client);
    void handleBrickGetNetworkSettings(BrickClient *client);
    void handleBrickSaveNetworkSettings(BrickClient *client, const std::string &ssid, const std::string &pwd);
    void handleBrickSaveBrickName(BrickClient *client, const std::string &brickName);

    void handleWiFiStationModeConnected(const WiFiEventStationModeConnected &);
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

    virtual BrickClient* createBrickClient(AsyncClient * tcpClient) = 0;
    virtual void deleteBrickClient(BrickClient * tcpClient) = 0;
    virtual std::string getBrickType() const = 0;
    virtual void update();

protected:
    BrickServer brickServer;

private:
#define AP_CONFIG_SSID "Aspoo Brick Config Network"
#define AP_CONFIG_PASSWORD "12345678"
    AppState appState = RUN_STATE;

    Preferences prefs;
    WiFiEventHandler networkConnectedHandler;
};