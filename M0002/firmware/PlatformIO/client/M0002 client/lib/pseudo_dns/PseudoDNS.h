#pragma once

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <string>
#include <functional>
#include <set>
#include <utility>

#include "ProtocolStd.h"

class PseudoDNS
{
  using CallbackFunction = std::function<void(const std::string &, const std::string &)>;
  using CallbackMethod = std::function<void(const std::string &, const std::string &)>;

public:
  void setHostName(const std::string &hostName);

  void run(const std::string &brickId, const std::string &brickType, const std::string &brickName);
  bool isRunning();
  void stopRunning();

  void startQueriesForAllHosts();
  bool isQueriesRunning();
  void stopQueries();

  void update();

  void onHostFound(CallbackFunction callback)
  {
    callbackFunction = callback;
  }

  template <typename T>
  void onHostFound(T *obj, void (T::*method)(const std::string &, const std::string &))
  {
    callbackMethod = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2);
  }

private:
  void onQueryTime();
  void onUdpDatagram(int packetSize);
  IPAddress softAPbroadcastIP();

private:
  std::set<std::pair<std::string, std::string>> dnsDiscoverdHosts; // string - hostName, string - hostIP
  CallbackMethod callbackMethod;
  CallbackFunction callbackFunction;
  std::string mId;
  std::string mType;
  std::string mName;
  bool mRunning = false;
  bool runQuery = false;
  uint16_t PORT = 6353;
  const unsigned long QUERY_INTERVAL = 2500;
  unsigned long lastQueryTime = 0;
  WiFiUDP Udp;
  unsigned long lastTime = 0;
  const unsigned long interval = 100;
  ProtocolStd protocolStd;
};