#include "network.h"
#include <ESP8266WiFi.h>



#ifndef APSSID
#define APSSID "Espoo_M0001"
#define APPSK "12345678"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

void setupAP(){
  // Begin WiFi AP
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

}

// Set WiFi credentials
#define WIFI_SSID "AndroidAP"
#define WIFI_PASS "wicl3264"
// #define WIFI_SSID "Espoo_M0001"
// #define WIFI_PASS "12345678"

void setupWiFi(){
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
}

bool wifiConnected(){
  return (!(WiFi.status() != WL_CONNECTED));
}