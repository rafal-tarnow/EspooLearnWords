#include "Application.h"
#include <ESP8266WiFi.h>
#include <sstream>
#include "T0002.hpp"
#include "J0001Client.hpp"
#include "K0002Client.hpp"
#include "M0004.hpp"
#include "K0007.hpp"

template class Application<T0002>;
template class Application<K0002Client>;
template class Application<M0004>;
template class Application<K0007>;
template class Application<J0001Client>;

using namespace std;

template <typename T>
Application<T>::Application() : stateManager(this)
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
}

template <typename T>
void Application<T>::begin()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    kikoLib.setup(&prefs, &stateManager);
    kikoLib.onNewBrickClient(this, &Application::handleNewBrickClient);

    stateManager.changeState(ApplicationStateManager<T>::RUN_STATE);

    // tcpServer = new TcpServer();
    // tcpServer->onNewConnection(this, &Application::handleTcpServerNewConnection);
    // tcpServer->listen(2883);

    setup();
}

template <typename T>
void Application<T>::handleButtonLongPress()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    stateManager.handleButtonLongPress();
}

template <typename T>
void Application<T>::handleButtonClick()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    stateManager.handleButtonClick();
}

template <typename T>
void Application<T>::handleButtonDoubleClick()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    stateManager.handleButtonDoubleClick();
}

template <typename T>
void Application<T>::cleanup()
{
    auto previousSize = kikoLib.clients.size();
    for (auto it = kikoLib.clientsToDelete.begin(); it != kikoLib.clientsToDelete.end(); ++it)
    {
        T *client = *it;
        kikoLib.clients.erase(client);
        delete client;
        client = nullptr;
    }
    kikoLib.clientsToDelete.clear();

    if (previousSize > 0 && kikoLib.clients.size() == 0)
    {
        std::string configReadBrickName();
        stateManager.handleAllTcpClientsDisconnected();
    }
}

template <typename T>
void Application<T>::startTcpServer()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    // asyncServer->begin();
}

template <typename T>
void Application<T>::stopTcpServer()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    // asyncServer->end();
}

template <typename T>
void Application<T>::wifiCreateAccesPoint(const std::string &ssid, const std::string &pwd)
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    if (pwd == "") // create accespoint without password
    {
        while (!WiFi.softAP(string("KIKO " + kikoLib.getType() + " Config Network").c_str(), nullptr, 6, false, 15))
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
}

template <typename T>
void Application<T>::tcpDisconnectAllClients()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
    {
        T *client = *it;
        client->close();
#ifdef __DBG_APPLICATION__
        Serial.printf("%lu client->close();\n", dtime());
#endif
    }
}

template <typename T>
void Application<T>::update()
{
    // Serial.printf("\n%lu --->>> Application::update()", dtime());
    kikoLib.loop();
    cleanup();

    if (BrickClient::isAnyWaiting())
    {
        // Serial.printf("\n%lu A", dtime());
        BrickClient::tryFlushBuffers();
    }

    if (!BrickClient::isAnyWaiting())
    {
        loop();
    }
}
