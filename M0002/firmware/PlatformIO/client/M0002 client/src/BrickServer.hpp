
#pragma once
#include <ESPAsyncTCP.h>
#include <Arduino.h>
#include <functional>
#include <string>
#include <utility>
#include <map>
#include "PseudoDNS.h"
#include "BrickClient.hpp"

class BrickServer
{

  using CallbackMethodCreate = std::function<BrickClient* (AsyncClient *)>;
  using CallbackMethodDelete = std::function<void (BrickClient *)>;

public:
  ~BrickServer();
  void begin(const std::string &brickName);

  template <typename T>
  void onBrickClientCreate(T *obj, BrickClient *(T::*method)(AsyncClient *))
  {
    callbackBrickCreate = std::bind(method, obj, std::placeholders::_1);
  }

    template <typename T>
  void onBrickClientDelete(T *obj, void (T::*method)(BrickClient *))
  {
    callbackBrickDelete = std::bind(method, obj, std::placeholders::_1);
  }

  void setBrickName(const std::string &brickName);
  void cmdSetBrickName(AsyncClient *client, const std::string &brickName);
  void cmdSetNetworkSettings(AsyncClient *client, const std::string &ssid, const std::string &pswd);
  bool isSomeoneConnected();
  void update();

private:
  // server events
  void onHandleNewClient(void *arg, AsyncClient *client);
  // clients events
  void handleTcpData(void *arg, AsyncClient *client, void *data, size_t len);
  void handleTcpError(void *arg, AsyncClient *client, int8_t error);
  void handleTcpDisconnect(void *arg, AsyncClient *client);
  void handleTcpTimeOut(void *arg, AsyncClient *client, uint32_t time);

private:
  CallbackMethodCreate callbackBrickCreate;
    CallbackMethodDelete callbackBrickDelete;
  uint16_t PORT = 2883;
  AsyncServer *mServer;
  PseudoDNS pseudoDNS;
  std::map<AsyncClient*, BrickClient*> brickClients;
};
