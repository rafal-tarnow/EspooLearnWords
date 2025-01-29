#pragma once

#include <ESP8266WiFi.h>
#include <Preferences.h>
#include <string>
#include <ESPAsyncTCP.h>
#include "ApplicationStateManager.hpp"
#include "TcpServer.hpp"
#include "PseudoDNS.h"
#include "config.h"
#include "BrickClient.hpp"

template <typename T>
class KikoBrickLib
{
  using CallbackBrickClient = std::function<void(T *)>;

public:
  void setup(Preferences *prefs, ApplicationStateManager<T> *stateManager);

  template <typename C>
  void onNewBrickClient(C *obj, void (C::*method)(T *))
  {
    callbackNewBrickClient = std::bind(method, obj, std::placeholders::_1);
  }
  void loop();

public:
  void wifiConnectToNetwork(const std::string &ssid, const std::string &pwd);
  void wifiConnectToConfigNetwork();
  void wifiDisconnectFromNetwork();
  void wifiDisconnectAccesPoint();

  void wifiDisconnectAll();
  std::string getType();
  std::string getId();
  PseudoDNS pseudoDNS;
  std::set<T *> clients;
  std::set<T *> clientsToDelete;

  void saveInConfig(std::string key, float value);
  void saveInConfig(std::string key, uint8_t value);

  uint8_t readFromConfig(std::string key, uint8_t defaultValue);
  float readFromConfig(std::string key, float defaultValue);

private:
  void handleWiFiStationModeConnected(const WiFiEventStationModeConnected &);
  void handleWiFiStationModeGotIp(const WiFiEventStationModeGotIP &);

  // tcp AsyncServer events
  void handleNewTcpClient(void *arg, AsyncClient *client);

  // TcpServer events
  void handleTcpServerNewConnection();
  void handleNewTcpSocket(TcpSocket *socket);
  void setupNewBrickClientCallbacks(BrickClient *);

  // brick client events
  void hangleGetBrickInfo(BrickClient *client);
  void handleGetBrickId(BrickClient *client);
  void handleGetBrickType(BrickClient *client);
  void handleGetBrickName(BrickClient *client);
  void handleBrickGetNetworkSettings(BrickClient *client);
  void handleBrickSaveNetworkSettings(BrickClient *client, const std::string &ssid, const std::string &pwd);
  void handleBrickSaveBrickName(BrickClient *client, const std::string &brickName);
  void handleDisconnectedClient(BrickClient *client);

  std::string getName();
  std::string getSsid();
  std::string getPswd();

  void configReadBrickName(std::string &brickName);
  std::string configReadBrickName();
  void configReadNetworkSettings(std::string &ssid, std::string &pwd);

  void configSaveNetworkSettings(const std::string &ssid, const std::string &pwd);
  void configSaveBrickName(const std::string &brickName);

private:
  CallbackBrickClient callbackNewBrickClient;
  WiFiEventHandler networkConnectedHandler;
  WiFiEventHandler stationModeGotIpHandler;
  Preferences *prefs;
  ApplicationStateManager<T> *stateManager;
  AsyncServer *asyncServer = nullptr;
  TcpServer *tcpServer;
  uint32_t connections;
  uint32_t disconnections;
};