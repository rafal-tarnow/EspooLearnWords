#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#include <OneButton.h>
#include "led.h"
#include "ApplicationB0002.h"
#include "ApplicationT0002.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "config.h"
#include "Debug.hpp"

#define BUTTON_PIN 0
#define LED_PIN 2

OneButton button = OneButton(
    BUTTON_PIN, // Input pin for the button
    true,       // Button is active LOW
    false       // Enable internal pull-up resistor
);

#ifdef __BRICK_B0002__
ApplicationB0002 application;
#elif defined(__BRICK_T0002__)
ApplicationT0002 application;
#endif


void setupOTA();

void handleLongPress()
{
  application.handleButtonLongPress();
}

void handleClick()
{
  application.handleButtonClick();
}

void handleDoubleClick()
{
  application.handleButtonDoubleClick();
}

void setup()
{
  // Setup serial
  Serial.begin(115200);
  delay(100);

  // Setup led
  LedInit(LED_PIN);
  LedSetState(false);

  button.attachLongPressStart(handleLongPress);
  button.attachClick(handleClick);
  button.attachDoubleClick(handleDoubleClick);
  button.setPressMs(7000);

  application.begin();

  setupOTA();
}

void loop()
{
  application.update();
  button.tick();
  LedTick();
  ArduinoOTA.handle();
  ObjectCounter::printArduino();
}

void setupOTA()
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });

  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });

  ArduinoOTA.begin();
}