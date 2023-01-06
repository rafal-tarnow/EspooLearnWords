#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "diode.h"

// Set WiFi credentials
#define WIFI_SSID "AndroidAP"
#define WIFI_PASS "wicl3264"


#ifndef APSSID
#define APSSID "Espoo_M0001"
#define APPSK "12345678"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;



//UDP
WiFiUDP UDP;
#define UDP_PORT 45455
char packet[6400];
char reply[] = "Hello, I'm M0001 module! :)";

void setup() {
  initLED();
  // Setup serial port
  Serial.begin(115200);
  Serial.println();




  // // Begin WiFi
  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  // // Connecting to WiFi...
  // Serial.print("Connecting to ");
  // Serial.print(WIFI_SSID);
  // // Loop continuously while WiFi is not connected
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(100);
  //   Serial.print(".");
  // }
  // // Connected to WiFi
  // Serial.println();
  // Serial.print("Connected! IP address: ");
  // Serial.println(WiFi.localIP());
  //


  // Begin WiFi AP
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
}

void loop() {
  // if (WiFi.status() != WL_CONNECTED) {
  //   LED(false);
  // } else {
  LED(true);
  // }

  // Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  // delay(3000);

  static int packageIndex = 0;

  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet ");
    Serial.print(packageIndex++);
    Serial.print("! Size: ");
    Serial.println(packetSize);
    int len = UDP.read(packet, 255);
    if (len > 0) {

      packet[len] = '\0';
    }
    // Serial.print("Packet received: ");
    // Serial.println(packet);

    // Send return packet
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(packet,4);
    //UDP.write(reply);
    UDP.endPacket();
  }
}