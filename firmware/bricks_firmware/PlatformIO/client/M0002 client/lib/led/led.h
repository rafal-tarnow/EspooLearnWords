#pragma once

void LedInit(int pin);
void LedBlink(int ms); // blink led
void LedSetState(bool state); //turn on off led
void LedTick(); //tick to update led state, called every loop()