#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "Brick.hpp"

#define SSID "ESP-TEST"
#define PASSWORD "12345678"


Brick brick;

void setup()
{
  Serial.begin(115200);
  delay(5000);

  // create access point
  while (!WiFi.softAP(SSID, PASSWORD, 6, false, 15))
  {
    delay(500);
  }
  
  Serial.println(" Started WiFi AP ");
  Serial.println(" IP = " + WiFi.softAPIP().toString());

  brick.begin();
}

void loop()
{
  
}