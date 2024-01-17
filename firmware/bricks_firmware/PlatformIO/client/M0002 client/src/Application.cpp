#include "Application.h"
#include <ESP8266WiFi.h>
#include <sstream>
#include "T0002.hpp"
#include "M0002.hpp"
#include "M0004.hpp"

template class Application<T0002>;
template class Application<M0002>;
template class Application<M0004>;

using namespace std;

template <typename T>
void Application<T>::begin()
{
    networkConnectedHandler = WiFi.onStationModeConnected(std::bind(&Application::handleWiFiStationModeConnected, this, std::placeholders::_1));
    stationModeGotIpHandler = WiFi.onStationModeGotIP(std::bind(&Application::handleWiFiStationModeGotIp, this, std::placeholders::_1));

    wifiConnectToConfigNetwork();

    string brickName = configReadBrickName();

    pseudoDNS.run(getId(), getType(), brickName);

    asyncServer = new AsyncServer(2883);
    asyncServer->onClient(std::bind(&Application::handleNewTcpClient, this, std::placeholders::_1, std::placeholders::_2), asyncServer);
    asyncServer->begin();

    // tcpServer = new TcpServer();
    // tcpServer->onNewConnection(this, &Application::handleTcpServerNewConnection);
    // tcpServer->listen(2883);
    setup();
}

template <typename T>
void Application<T>::handleButtonLongPress()
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

template <typename T>
void Application<T>::handleButtonClick()
{
    if (appState == CONFIG_STATE)
    {
        appState = RUN_STATE;
        wifiDisconnectAll();
        wifiConnectToConfigNetwork();
    }
}

template <typename T>
void Application<T>::handleButtonDoubleClick()
{
    if (appState == CONFIG_STATE)
    {
        appState = RUN_STATE;
        wifiDisconnectAll();
        wifiConnectToConfigNetwork();
    }
}

template <typename T>
void Application<T>::setupNewBrickClientCallbacks(BrickClient *brickClient)
{
    brickClient->onGetInfo(this, &Application::hangleGetBrickInfo);
    brickClient->onGetId(this, &Application::handleGetBrickId);
    brickClient->onGetType(this, &Application::handleGetBrickType);
    brickClient->onGetBrickName(this, &Application::handleGetBrickName);
    brickClient->onGetNetworkSetting(this, &Application::handleBrickGetNetworkSettings);
    brickClient->onSaveNetworkSetting(this, &Application::handleBrickSaveNetworkSettings);
    brickClient->onSaveBrickName(this, &Application::handleBrickSaveBrickName);

    brickClient->onTcpDisconnected(this, &Application::handleDisconnectedClient);
}

template <typename T>
void Application<T>::handleDisconnectedClient(BrickClient *brickClient)
{
    Serial.printf("\n%lu Application::handleDisconnectedClient()", dtime());
    disconnections++;
    clientsToDelete.insert(reinterpret_cast<T *>(brickClient));
}

template <typename T>
void Application<T>::cleanup()
{
    for (auto it = clientsToDelete.begin(); it != clientsToDelete.end(); ++it)
    {
        T *client = *it;
        clients.erase(client);
        delete client;
        client = nullptr;
    }
    clientsToDelete.clear();
}

template <typename T>
std::string Application<T>::getId()
{
    uint32_t eid = ESP.getChipId();

    std::ostringstream oss;
    oss << eid;
    std::string result = getType() + oss.str();

    return result;
}

template <typename T>
std::string Application<T>::getType()
{
    return T::type();
}

template <typename T>
std::string Application<T>::getName()
{
    // Serial.printf("\n%lu Application::handleGetBrickName() ", dtime());
    string brickName;
    // Serial.printf("\n%lu string brickName %s", dtime(), brickName.c_str());
    configReadBrickName(brickName);
    return brickName;
}

template <typename T>
std::string Application<T>::getSsid()
{
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    return ssid;
}

template <typename T>
std::string Application<T>::getPswd()
{
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    return pwd;
}

template <typename T>
void Application<T>::handleNewTcpClient(void *arg, AsyncClient *tcpClient)
{
    // Serial.printf("\n%lu Application::handleNewTcpClient()", dtime());
    connections++;
    T *newClient = new T(tcpClient);
    setupNewBrickClientCallbacks(newClient);
    clients.insert(newClient);
}

template <typename T>
void Application<T>::handleNewTcpSocket(TcpSocket *socket)
{
    Serial.printf("\n%lu ApplicationT0002::handleNewTcpSocket()", dtime());
    T *newClient = new T(socket);
    setupNewBrickClientCallbacks(newClient);
    clients.insert(newClient);
}

template <typename T>
void Application<T>::hangleGetBrickInfo(BrickClient *client)
{
    Serial.printf("\nApplication<T>::hangleGetBrickInfo()");
    client->cmdSetInfo(getId(), getType(), getName(), getSsid(), getPswd());
}

template <typename T>
void Application<T>::handleGetBrickId(BrickClient *client)
{
    client->cmdSetId(getId());
}

template <typename T>
void Application<T>::handleGetBrickType(BrickClient *client)
{
    client->cmdSetType(getType());
}

template <typename T>
void Application<T>::handleGetBrickName(BrickClient *client)
{
    client->cmdSetName(getName());
    // Serial.printf("\n%lu client->cmdSetBrickNameAndType(brickName, T::type()); ", dtime());
}

template <typename T>
void Application<T>::handleBrickGetNetworkSettings(BrickClient *client)
{
    Serial.println("Application::handleBrickGetNetworkSettings()");
    client->cmdSetNetworkSettings(getSsid(), getPswd());
}

template <typename T>
void Application<T>::handleBrickSaveNetworkSettings(BrickClient *client, const std::string &ssid, const std::string &pwd)
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

template <typename T>
void Application<T>::handleBrickSaveBrickName(BrickClient *client, const std::string &brickName)
{
    Serial.println("Application::handleBrickSaveNetworkSettings()");
    pseudoDNS.setHostName(brickName);
    configSaveBrickName(brickName);
    client->cmdSetName(getName());
}

template <typename T>
void Application<T>::handleWiFiStationModeConnected(const WiFiEventStationModeConnected &)
{
    Serial.printf("\n%lu Application::handleWiFiStationModeConnected()", dtime());
}

template <typename T>
void Application<T>::handleWiFiStationModeGotIp(const WiFiEventStationModeGotIP &)
{
    Serial.printf("\n%lu Application::handleWiFiStationModeGotIp()", dtime());
    Serial.printf("\n%lu Adres IP urządzenia: ", dtime());
    Serial.printf("\n%lu %s", dtime(), WiFi.localIP().toString().c_str());
    LedSetState(true);
}

template <typename T>
void Application<T>::wifiConnectToConfigNetwork()
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

template <typename T>
void Application<T>::wifiConnectToNetwork(const std::string &ssid, const std::string &pwd)
{
    // connects to access point
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pwd.c_str());
}

template <typename T>
void Application<T>::wifiCreateAccesPoint(const std::string &ssid, const std::string &pwd)
{
    if (pwd == "") // create accespoint without password
    {
        while (!WiFi.softAP(string("Aspoo " + getType() + " Config Network").c_str(), nullptr, 6, false, 15))
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

template <typename T>
void Application<T>::wifiDisconnectFromNetwork()
{
    WiFi.disconnect();
}

template <typename T>
void Application<T>::wifiDisconnectAccesPoint()
{

    WiFi.softAPdisconnect();
}

template <typename T>
void Application<T>::wifiDisconnectAll()
{
    wifiDisconnectFromNetwork();
    wifiDisconnectAccesPoint();
}

template <typename T>
std::string Application<T>::configReadBrickName()
{
    prefs.begin("ASPOO_BRICK");
    string brickName = prefs.getString("BRICK_NAME", std::string("Aspoo" + getType()).c_str()).c_str(); // + T::type()).c_str();
    prefs.end();
    return brickName;
}

template <typename T>
void Application<T>::configReadBrickName(std::string &brickName)
{
    Serial.printf("\n%lu Application::configReadBrickName()", dtime());
    prefs.begin("ASPOO_BRICK");
    // Serial.printf("\nprefs.begin(\"ASPOO_BRICK\");");
    brickName = prefs.getString("BRICK_NAME", std::string("Aspoo" + getType()).c_str()).c_str(); // + T::type()).c_str();
    // Serial.printf("\npbrickName = prefs.getString");
    prefs.end();
    Serial.printf("\n%lu END Application::configReadBrickName()", dtime());
}

template <typename T>
void Application<T>::configReadNetworkSettings(std::string &ssid, std::string &pwd)
{
    prefs.begin("ASPOO_BRICK");
    ssid = prefs.getString("SSID", "DefaultSSID").c_str();
    pwd = prefs.getString("PWD", "DefaultPWD").c_str();
    prefs.end();
}

template <typename T>
void Application<T>::configSaveNetworkSettings(const std::string &ssid, const std::string &pwd)
{
    Serial.println("Application::configSaveNetworkSettings()");
    prefs.begin("ASPOO_BRICK");
    prefs.putString("SSID", String(ssid.c_str()));
    prefs.putString("PWD", String(pwd.c_str()));
    prefs.end();
}

template <typename T>
void Application<T>::configSaveBrickName(const std::string &brickName)
{
    Serial.println("Application::configSaveBrickName()");
    prefs.begin("ASPOO_BRICK");
    prefs.putString("BRICK_NAME", String(brickName.c_str()));
    prefs.end();
}

template <typename T>
void Application<T>::handleTcpServerNewConnection()
{
    while (tcpServer->hasPendingConnections())
    {
        handleNewTcpSocket(tcpServer->nextPendingConnection());
    }
}

template <typename T>
void Application<T>::update()
{
    // Serial.printf("\n%lu --->>> Application::update()", dtime());

    pseudoDNS.update();
    cleanup();

    if (BrickClient::isAnyWaiting())
    {
        //Serial.printf("\n%lu A", dtime());
        BrickClient::tryFlushBuffers();
    }

    if (!BrickClient::isAnyWaiting())
    {
        loop();
    }
}
