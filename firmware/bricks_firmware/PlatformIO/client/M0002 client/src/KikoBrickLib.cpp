#include "KikoBrickLib.hpp"
#include <led.h>
#include <sstream>
#include "dtime.h"
#include "T0002.hpp"
#include "K0002Client.hpp"
#include "M0004.hpp"
#include "K0007.hpp"

using namespace std;

template class KikoBrickLib<T0002>;
template class KikoBrickLib<K0002Client>;
template class KikoBrickLib<M0004>;
template class KikoBrickLib<K0007>;

template <typename T>
void KikoBrickLib<T>::setup(Preferences *pref, ApplicationStateManager<T> *stateMgr)
{
    prefs = pref;
    stateManager = stateMgr;

    networkConnectedHandler = WiFi.onStationModeConnected(std::bind(&KikoBrickLib::handleWiFiStationModeConnected, this, std::placeholders::_1));
    stationModeGotIpHandler = WiFi.onStationModeGotIP(std::bind(&KikoBrickLib::handleWiFiStationModeGotIp, this, std::placeholders::_1));

    string brickName = configReadBrickName();
    pseudoDNS.run(getId(), getType(), brickName);

    asyncServer = new AsyncServer(2883);
    asyncServer->onClient(std::bind(&KikoBrickLib::handleNewTcpClient, this, std::placeholders::_1, std::placeholders::_2), asyncServer);
    asyncServer->begin();
}

template <typename T>
void KikoBrickLib<T>::loop()
{
    pseudoDNS.update();
}

template <typename T>
void KikoBrickLib<T>::handleWiFiStationModeConnected(const WiFiEventStationModeConnected &)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
}

template <typename T>
void KikoBrickLib<T>::handleWiFiStationModeGotIp(const WiFiEventStationModeGotIP &)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    LedSetState(true);
}

template <typename T>
std::string KikoBrickLib<T>::configReadBrickName()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    prefs->begin("KIKO_BRICK");
    string brickName = prefs->getString("BRICK_NAME", std::string("KIKO " + getType()).c_str()).c_str(); // + T::type()).c_str();
    prefs->end();
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu brickName=%s\n", dtime(), brickName.c_str());
#endif
    return brickName;
}

template <typename T>
std::string KikoBrickLib<T>::getType()
{
    string type = T::type();
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    Serial.printf("%lu type=%s\n", dtime(), type.c_str());
#endif
    return type;
}

template <typename T>
std::string KikoBrickLib<T>::getId()
{

    uint32_t eid = ESP.getChipId();

    std::ostringstream oss;
    oss << eid;
    std::string result = getType() + oss.str();

#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    Serial.printf("%lu id=%s\n", dtime(), result.c_str());
#endif
    return result;
}


template <typename T>
void KikoBrickLib<T>::handleNewTcpClient(void *arg, AsyncClient *tcpClient)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    if (stateManager->mAppState == ApplicationStateManager<T>::TRANSITION_FROM_RUN_TO_CONFIG_STATE)
    {
        tcpClient->close();
    }
    // Serial.printf("\n%lu Application::handleNewTcpClient()", dtime());
    connections++;
    T *newClient = new T(tcpClient);
    setupNewBrickClientCallbacks(newClient);
    clients.insert(newClient);
    if(callbackNewBrickClient){
        callbackNewBrickClient(newClient);
    }
}

template <typename T>
void KikoBrickLib<T>::handleTcpServerNewConnection()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    while (tcpServer->hasPendingConnections())
    {
        handleNewTcpSocket(tcpServer->nextPendingConnection());
    }
}

template <typename T>
void KikoBrickLib<T>::setupNewBrickClientCallbacks(BrickClient *brickClient)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    brickClient->onGetInfo(this, &KikoBrickLib::hangleGetBrickInfo);
    brickClient->onGetId(this, &KikoBrickLib::handleGetBrickId);
    brickClient->onGetType(this, &KikoBrickLib::handleGetBrickType);
    brickClient->onGetBrickName(this, &KikoBrickLib::handleGetBrickName);
    brickClient->onGetNetworkSetting(this, &KikoBrickLib::handleBrickGetNetworkSettings);
    brickClient->onSaveNetworkSetting(this, &KikoBrickLib::handleBrickSaveNetworkSettings);
    brickClient->onSaveBrickName(this, &KikoBrickLib::handleBrickSaveBrickName);

    brickClient->onTcpDisconnected(this, &KikoBrickLib::handleDisconnectedClient);
}

template <typename T>
void KikoBrickLib<T>::handleNewTcpSocket(TcpSocket *socket)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    if (stateManager->mAppState == ApplicationStateManager<T>::TRANSITION_FROM_RUN_TO_CONFIG_STATE)
    {
        socket->close();
    }
    T *newClient = new T(socket);
    setupNewBrickClientCallbacks(newClient);
    clients.insert(newClient);
}







template <typename T>
void KikoBrickLib<T>::hangleGetBrickInfo(BrickClient *client)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    client->cmdSetInfo(getId(), getType(), getName(), getSsid(), getPswd());
}

template <typename T>
void KikoBrickLib<T>::handleGetBrickId(BrickClient *client)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    client->cmdSetId(getId());
}

template <typename T>
void KikoBrickLib<T>::handleGetBrickType(BrickClient *client)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    client->cmdSetType(getType());
}

template <typename T>
void KikoBrickLib<T>::handleGetBrickName(BrickClient *client)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    client->cmdSetName(getName());
    // Serial.printf("\n%lu client->cmdSetBrickNameAndType(brickName, T::type()); ", dtime());
}

template <typename T>
void KikoBrickLib<T>::handleBrickGetNetworkSettings(BrickClient *client)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    client->cmdSetNetworkSettings(getSsid(), getPswd());
}

template <typename T>
void KikoBrickLib<T>::handleBrickSaveNetworkSettings(BrickClient *client, const std::string &newSsid, const std::string &newPwd)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif

    string currentSsid;
    string currentPwd;
    configReadNetworkSettings(currentSsid, currentPwd);
    if (newSsid != currentSsid || newPwd != currentPwd)
    {

        configSaveNetworkSettings(newSsid, newPwd);
        if (stateManager->mAppState == ApplicationStateManager<T>::CONFIG_STATE)
        {
            stateManager->mAppState = ApplicationStateManager<T>::RUN_STATE;
            wifiDisconnectAll();
            wifiConnectToConfigNetwork();
        }
        else if (stateManager->mAppState == ApplicationStateManager<T>::RUN_STATE)
        {
            wifiDisconnectAll();
            wifiConnectToConfigNetwork();
        }
    }
}

template <typename T>
void KikoBrickLib<T>::handleBrickSaveBrickName(BrickClient *client, const std::string &brickName)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    pseudoDNS.setHostName(brickName);
    configSaveBrickName(brickName);
    client->cmdSetName(getName());
}

template <typename T>
void KikoBrickLib<T>::handleDisconnectedClient(BrickClient *brickClient)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    disconnections++;
    clientsToDelete.insert(reinterpret_cast<T *>(brickClient));
}

template <typename T>
std::string KikoBrickLib<T>::getName()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    string brickName;
    // Serial.printf("\n%lu string brickName %s", dtime(), brickName.c_str());
    configReadBrickName(brickName);
    return brickName;
}

template <typename T>
std::string KikoBrickLib<T>::getSsid()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    return ssid;
}

template <typename T>
std::string KikoBrickLib<T>::getPswd()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    return pwd;
}

template <typename T>
void KikoBrickLib<T>::configReadNetworkSettings(std::string &ssid, std::string &pwd)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    prefs->begin("KIKO_BRICK");
    ssid = prefs->getString("SSID", "DefaultSSID").c_str();
    pwd = prefs->getString("PWD", "DefaultPWD").c_str();
    prefs->end();
}

template <typename T>
void KikoBrickLib<T>::configReadBrickName(std::string &brickName)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    prefs->begin("KIKO_BRICK");
    brickName = prefs->getString("BRICK_NAME", std::string("KIKO " + getType()).c_str()).c_str(); // + T::type()).c_str();
    prefs->end();
}

template <typename T>
void KikoBrickLib<T>::configSaveNetworkSettings(const std::string &ssid, const std::string &pwd)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    prefs->begin("KIKO_BRICK");
    prefs->putString("SSID", String(ssid.c_str()));
    prefs->putString("PWD", String(pwd.c_str()));
    prefs->end();
}

template <typename T>
void KikoBrickLib<T>::configSaveBrickName(const std::string &brickName)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    prefs->begin("KIKO_BRICK");
    prefs->putString("BRICK_NAME", String(brickName.c_str()));
    prefs->end();
}

template <typename T>
void KikoBrickLib<T>::wifiDisconnectAll()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    wifiDisconnectFromNetwork();
    wifiDisconnectAccesPoint();
}


template <typename T>
void KikoBrickLib<T>::wifiDisconnectFromNetwork()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    WiFi.disconnect();
}

template <typename T>
void KikoBrickLib<T>::wifiDisconnectAccesPoint()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    WiFi.softAPdisconnect();
}

template <typename T>
void KikoBrickLib<T>::wifiConnectToConfigNetwork()
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    string ssid;
    string pwd;
    configReadNetworkSettings(ssid, pwd);
    Serial.println(ssid.c_str());
    Serial.println(pwd.c_str());
    wifiConnectToNetwork(ssid, pwd);
    LedBlink(1000);
}


template <typename T>
void KikoBrickLib<T>::wifiConnectToNetwork(const std::string &ssid, const std::string &pwd)
{
#ifdef __DBG_KIKO_BRICK_LIB__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pwd.c_str());
}
