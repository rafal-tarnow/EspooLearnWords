#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>

  
class Device {
public:
  void setup();
  void loop();
private:
  void parseStatusCommand(char *packet);
  //UDP
  WiFiUDP UDP;
#define UDP_PORT 45455
  char packet[6400];

};