#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>
#include <Preferences.h>

  
class Device {
public:
  void setup();
  void loop();
private:
  void parseStatusCommand(char *packet);
  void parseNetworkConfigCommand(char * packet);
  Preferences preferences;
  //UDP
  WiFiUDP UDP;
#define UDP_PORT 45455
  char packet[6400];

};