#include <ESP8266WiFi.h>
#include <arduino-timer.h>
#include <Preferences.h>
#include "diode.h"
#include "device.h"
#include "network.h"


//WS2812B
//button D3
//pixel D4




Device device;
static Preferences preferences;


auto timer = timer_create_default();
bool timer_event(void *);

void setup() {

  preferences.begin("M0001", true);
  bool apMode = preferences.getBool("APMode", 0);
  preferences.end();

  initOnOff();
  // Setup serial port
  Serial.begin(921600);
  Serial.println();



  if (apMode) {
    setupAP();
  } else {
    setupWiFi();
  }


  device.setup();

  //timer.every(2 * 60 * 1000, timer_event);
}

bool timer_event(void *) {
  static bool led_state = false;
  led_state = !led_state;
  if (led_state) {
    setupAP();
  } else {
    setupWiFi();
  }
  return true;
}


void loop() {
  device.loop();
  timer.tick();  
}
