#pragma once
#include <led.h>
#include <Preferences.h>
#include "ApplicationStateManager.hpp"
#include "BrickClient.hpp"
#include "TcpServer.hpp"
#include "KikoBrickLib.hpp"
#include "Timer.hpp"

template <typename T>
class Application
{
    friend class ApplicationStateManager<T>;

public:
    Application();
    virtual ~Application() = default;
    void begin();

    // config button event handlers
    void handleButtonLongPress();
    void handleButtonClick();
    void handleButtonDoubleClick();


    void handleWiFiStationModeConnected(const WiFiEventStationModeConnected &);
    void handleWiFiStationModeGotIp(const WiFiEventStationModeGotIP &);

   

    void wifiCreateAccesPoint(const std::string &ssid, const std::string &pwd);



    void tcpDisconnectAllClients();


 
   


    void update();
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void handleNewBrickClient(T *client) = 0;

protected:

    KikoBrickLib<T> kikoLib;

private:
    void startTcpServer();
    void stopTcpServer();

   

   

    void cleanup();


private:
#define AP_CONFIG_SSID "KIKO Brick Config Network"
#define AP_CONFIG_PASSWORD "12345678"
    ApplicationStateManager<T> stateManager;

    uint16_t TCP_PORT = 2883;
    Preferences prefs;


};

#include "Application.tcc"