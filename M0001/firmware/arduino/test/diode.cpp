#include "diode.h"
#include <Arduino.h>


void initLED() {
  pinMode(LED_BUILTIN, OUTPUT);
  LED(false);
}

void LED(bool state){
  if(state){
    digitalWrite(LED_BUILTIN, LOW);
  }else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
}