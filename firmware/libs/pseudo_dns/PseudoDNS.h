#pragma once

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <string>
#include <functional>
#include <set>
#include <utility>
#include <map>

#include "ProtocolStd.h"

struct HostInfo
{
  std::string hostId;
  std::string hostType;
  std::string hostName;
  std::string hostIp;

  bool operator==(const HostInfo &other) const
  {
    return hostId == other.hostId &&
           hostType == other.hostType &&
           hostName == other.hostName &&
           hostIp == other.hostIp;
  }

  bool operator!=(const HostInfo &other) const
  {
    return !(*this == other); // Operator != jest przeciwnością operatora ==
  }
};

class PseudoDNS
{
  using CallbackFunction = std::function<void(const std::string &, const std::string &, const std::string &, const std::string &)>;
  using CallbackMethod = std::function<void(const std::string &, const std::string &, const std::string &, const std::string &)>;

public:
  void setHostName(const std::string &hostName);

  void run(const std::string &brickId, const std::string &brickType, const std::string &brickName);
  bool isRunning();
  void stopRunning();

  void startQueriesForAllHosts();
  bool isQueriesRunning();
  void stopQueries();

  void update();

  void onHostFound(CallbackFunction callback) //hostId, hostType, hostName, hostIP
  {
    callbackFunction = callback;
  }

  template <typename T>
  void onHostFound(T *obj, void (T::*method)(const std::string &, const std::string &, const std::string &, const std::string &)) //string hostName, string hostIP
  {
    callbackMethod = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
  }

  std::string getIPById(std::string id);

private:
  void onQueryTime();
  void onUdpDatagram(int packetSize);
  IPAddress softAPbroadcastIP();

private:
  std::map<std::string, HostInfo> dnsDiscoverdHosts; // key - hostId, value - HostInfo
  CallbackMethod callbackMethod;
  CallbackFunction callbackFunction;
  std::string mId;
  std::string mType;
  std::string mName;
  bool mRunning = false;
  bool runQuery = false;
  uint16_t PORT = 6353;
  const unsigned long QUERY_INTERVAL = 250;
  unsigned long lastQueryTime = 0;
  WiFiUDP Udp;
  unsigned long lastTime = 0;
  ProtocolStd protocolStd;
};