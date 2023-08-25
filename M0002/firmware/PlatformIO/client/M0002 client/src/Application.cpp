#include "Application.h"
#include <ESP8266WiFi.h>

using namespace std;

void Application::begin()
{
    networkConnectedHandler = WiFi.onStationModeConnected(std::bind(&Application::handleWiFiStationModeConnected, this, std::placeholders::_1));

    wifiConnectToConfigNetwork();

    brickServer.onBrickClientCreate(this, &Application::handleBrickClientCreate);
    brickServer.onBrickClientDelete(this, &Application::handleBrickClientDelete);
    brickServer.begin(configReadBrickName());
}

void Application::handleButtonLongPress()
{
    if (appState != CONFIG_STATE)
    {
        appState = CONFIG_STATE;
        LedBlink(150);
        // create access point
        wifiDisconnectAll();
        wifiCreateAccesPoint(AP_CONFIG_SSID, "");
    }
}

void Application::handleButtonClick()
{
    if (appState == CONFIG_STATE)
    {
        appState = RUN_STATE;
        wifiDisconnectAll();
        wifiConnectToConfigNetwork();
    }
}

void Application::handleButtonDoubleClick()
{
    if (appState == CONFIG_STATE)
    {
        appState = RUN_STATE;
        wifiDisconnectAll();
        wifiConnectToConfigNetwork();
    }
}

BrickClient *Application::handleBrickClientCreate(AsyncClient *tcpClient)
{
    BrickClient *brickClient = createBrickClient(tcpClient);
    brickClient->onGetNetworkSetting(this, &Application::handleBrickGetNetworkSettings);
    brickClient->onSaveNetworkSetting(this, &Application::handleBrickSaveNetworkSettings);
    brickClient->onSaveBrickName(this, &Application::handleBrickSaveBrickName);
    brickClient->onGetBrickName(this, &Application::handleGetBrickName);
    return brickClient;
}

void Application::handleBrickClientDelete(BrickClient *brickClient)
{
    deleteBrickClient(brickClient);
}

void Application::handleGetBrickName(BrickClient *client)
{
    Serial.println("Application::handleGetBrickName() ");
    string brickName;
    configReadBrickName(brickName);
    client->cmdSetBrickNameAndType(brickName, getBrickType());
}

void Application::handleBrickGetNetworkSettings(BrickClient *client)
{
    Serial.println("Application::handleBrickGetNetworkSettings()");
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    client->cmdSetNetworkSettings(ssid, pwd);
}

void Application::handleBrickSaveNetworkSettings(BrickClient *client, const std::string &ssid, const std::string &pwd)
{
    Serial.println("Application::handleBrickSaveNetworkSettings()");
    configSaveNetworkSettings(ssid, pwd);
    if (appState == CONFIG_STATE)
    {
        appState = RUN_STATE;
        wifiDisconnectAll();
        wifiConnectToConfigNetwork();
    }
}

void Application::handleBrickSaveBrickName(BrickClient *client, const std::string &brickName)
{
    Serial.println("Application::handleBrickSaveNetworkSettings()");
    brickServer.setBrickName(brickName);
    configSaveBrickName(brickName);
}

void Application::handleWiFiStationModeConnected(const WiFiEventStationModeConnected &)
{
    Serial.println("Application::handleWiFiStationModeConnected()");
    LedSetState(true);
}

void Application::wifiConnectToConfigNetwork()
{
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    Serial.println("Application::wifiConnectToConfigNetwork()");
    Serial.println(ssid.c_str());
    Serial.println(pwd.c_str());
    wifiConnectToNetwork(ssid, pwd);
    LedBlink(1000);
}

void Application::wifiConnectToNetwork(const std::string &ssid, const std::string &pwd)
{
    // connects to access point
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pwd.c_str());
}

void Application::wifiCreateAccesPoint(const std::string &ssid, const std::string &pwd)
{
    if (pwd == "") // create accespoint without password
    {
        while (!WiFi.softAP(string("Aspoo " + getBrickType() + " Config Network").c_str(), nullptr, 6, false, 15))
        {
            delay(500);
        }
    }
    else
    { // create accespoint with password
        while (!WiFi.softAP(ssid.c_str(), pwd.c_str(), 6, false, 15))
        {
            delay(500);
        }
    }
    Serial.println(" Started WiFi AP ");
    Serial.println(" IP = " + WiFi.softAPIP().toString());
}

void Application::wifiDisconnectFromNetwork()
{
    WiFi.disconnect();
}

void Application::wifiDisconnectAccesPoint()
{

    WiFi.softAPdisconnect();
}

void Application::wifiDisconnectAll()
{
    wifiDisconnectFromNetwork();
    wifiDisconnectAccesPoint();
}

std::string Application::configReadBrickName()
{
    prefs.begin("ASPOO_BRICK");
    string brickName = prefs.getString("BRICK_NAME", std::string("Aspoo" + getBrickType()).c_str()).c_str(); // + brick->getBrickType()).c_str();
    prefs.end();
    return brickName;
}

void Application::configReadBrickName(std::string &brickName)
{
    prefs.begin("ASPOO_BRICK");
    brickName = prefs.getString("BRICK_NAME", std::string("Aspoo" + getBrickType()).c_str()).c_str(); // + brick->getBrickType()).c_str();
    prefs.end();
    Serial.println((string("Application::configReadBrickName() brickName = ") + brickName).c_str());
}

void Application::configReadNetworkSettings(std::string &ssid, std::string &pwd)
{
    prefs.begin("ASPOO_BRICK");
    ssid = prefs.getString("SSID", "DefaultSSID").c_str();
    pwd = prefs.getString("PWD", "DefaultPWD").c_str();
    prefs.end();
}

void Application::configSaveNetworkSettings(const std::string &ssid, const std::string &pwd)
{
    Serial.println("Application::configSaveNetworkSettings()");
    prefs.begin("ASPOO_BRICK");
    prefs.putString("SSID", String(ssid.c_str()));
    prefs.putString("PWD", String(pwd.c_str()));
    prefs.end();
}

void Application::configSaveBrickName(const std::string &brickName)
{
    Serial.println("Application::configSaveBrickName()");
    prefs.begin("ASPOO_BRICK");
    prefs.putString("BRICK_NAME", String(brickName.c_str()));
    prefs.end();
}

void Application::update()
{
    // Serial.println("Application::update()");
    brickServer.update();
}
