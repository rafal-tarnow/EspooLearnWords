#include <ESP8266WiFi.h>
#include "diode.h"
#include "device.h"

// Set WiFi credentials
#define WIFI_SSID "AndroidAP"
#define WIFI_PASS "wicl3264"
// #define WIFI_SSID "Espoo_M0001"
// #define WIFI_PASS "12345678"


//WS2812B
//button D3
//pixel D4

#ifndef APSSID
#define APSSID "Espoo_M0001"
#define APPSK "12345678"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;



Device device;

void setup() {
  initLED();
  // Setup serial port
  Serial.begin(115200);
  Serial.println();




  // // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  //


  // Begin WiFi AP
  /* You can remove the password parameter if you want the AP to be open. */
  // WiFi.softAP(ssid, password);
  // IPAddress myIP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(myIP);


  device.setup();
}


void loop() {
  static bool setLedWifiStatus = false;
  if (setLedWifiStatus == false) {
    setLedWifiStatus = true;
    if (WiFi.status() != WL_CONNECTED) {
      LED(false);
    } else {
      LED(true);
    }
  }

  // Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  // delay(3000);

  device.loop();
}

