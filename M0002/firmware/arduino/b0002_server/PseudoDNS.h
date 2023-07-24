# pragma once

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <string>

class PseudoDNS{
  public:
  void begin();
  void startServer(std::string name);
  void stopServer();
  bool isRunning();
  void update();
private:
  void sendSearchPackage();
  IPAddress softAPbroadcastIP();
  private:
  std::string mName;
  bool mRunning = false;
  WiFiUDP Udp;
  unsigned long lastTime = 0;
  const unsigned long interval = 100;
};