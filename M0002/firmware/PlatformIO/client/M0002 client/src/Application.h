#pragma once
#include <led.h>
#include "BrickServer.hpp"
#include <Preferences.h>

class Application
{
    enum AppState
    {
        CONFIG_STATE,
        RUN_STATE,
    };

public:
    void begin(BrickServer *brick);
    void handleButtonLongPress();
    void handleButtonClick();
    void handleButtonDoubleClick();

    void handleGetBrickName();
    void handleBrickGetNetworkSettings();
    void handleBrickSaveNetworkSettings(const std::string &ssid, const std::string &pwd);
    void handleBrickSaveBrickName(const std::string &brickName);

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

    virtual void update();

private:
#define AP_CONFIG_SSID "Aspoo Brick Config Network"
#define AP_CONFIG_PASSWORD "12345678"
    AppState appState = RUN_STATE;
    BrickServer *brick;
    Preferences prefs;
    WiFiEventHandler networkConnectedHandler;
};