#include "PseudoDNS.h"

void PseudoDNS::begin() {
  Udp.begin(1234);
}

void PseudoDNS::startServer(std::string name) {
  mName = name;
  mRunning = true;
}

void PseudoDNS::stopServer() {
  mRunning = false;
}

bool PseudoDNS::isRunning() {
  return mRunning;
}

IPAddress PseudoDNS::softAPbroadcastIP() {
  struct ip_info ip;
  wifi_get_ip_info(SOFTAP_IF, &ip);
  return IPAddress(ip.ip.addr | ~(ip.netmask.addr));
}

void PseudoDNS::sendSearchPackage() {
  IPAddress broadcast;
  if (WIFI_AP == WiFi.getMode()) {
    broadcast = softAPbroadcastIP();
  }else{
    broadcast = WiFi.broadcastIP();
  }
  Udp.beginPacket(broadcast, 1234);
  Udp.write(mName.c_str());
  Udp.endPacket();
  Serial.printf("\n Send Pseudo DNS package");
}

void PseudoDNS::update() {
  if (mRunning) {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= interval) {
      sendSearchPackage();
      lastTime = currentTime;
    }
  }
}