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
  void loop();
private:
  void sendPseudoDNSPackage();
  private:
  std::string mName;
  bool mRunning = false;
  WiFiUDP Udp;
  IPAddress broadcastIP = IPAddress(255, 255, 255, 255);
  unsigned long lastTime = 0;
  const unsigned long interval = 100;
};