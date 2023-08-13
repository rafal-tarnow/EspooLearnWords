#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <OneButton.h>
#include "led.h"
#include "ApplicationB0002.h"
#include <Arduino.h>

#define BUTTON_PIN 0
#define LED_PIN 2

OneButton button = OneButton(
    BUTTON_PIN, // Input pin for the button
    true,       // Button is active LOW
    false       // Enable internal pull-up resistor
);

ApplicationB0002 application;

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
}

void loop()
{
  application.update();
  button.tick();
  LedTick();
}