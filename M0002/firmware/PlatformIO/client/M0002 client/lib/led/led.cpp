#include "led.h"
#include <Arduino.h>

static int ledPin;
static unsigned long previousMillis = 0;
static int ledState = HIGH; // Initial LED state: turned on
static unsigned long interval = 1000; // interval at which to blink (milliseconds)
static bool isBlinking = false; // Flag indicating if LED is in blinking mode

void LedInit(int pin) {
  ledPin = pin;
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, !ledState); // Set initial LED state
}

void LedBlink(int ms) {
  interval = ms;
  isBlinking = true;
}

void LedSetState(bool state) {
  digitalWrite(ledPin, !state);
  ledState = state;
  isBlinking = false;
}

void LedTick() {
  if (isBlinking) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      // Save the last time you blinked the LED
      previousMillis = currentMillis;

      // Toggle LED state
      ledState = !ledState;

      digitalWrite(ledPin, !ledState);
    }
  }
}
