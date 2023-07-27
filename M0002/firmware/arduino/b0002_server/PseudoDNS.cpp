#include "PseudoDNS.h"

void PseudoDNS::run(const std::string &myHostName) {
  mRunning = true;
  mName = myHostName;
  Udp.begin(PORT);
}

bool PseudoDNS::isRunning() {
  return mRunning;
}

void PseudoDNS::stopRunning() {
  mRunning = false;
  if (runQuery == false) {
    Udp.stop();
  }
}

void PseudoDNS::startQueriesForAllHosts() {
  runQuery = true;
  Udp.begin(PORT);
}

bool PseudoDNS::isQueriesRunning() {
  return runQuery;
}

void PseudoDNS::stopQueries() {
  runQuery = false;
  dnsDiscoverdHosts.clear();
  if (mRunning == false) {
    Udp.stop();
  }
}

IPAddress PseudoDNS::softAPbroadcastIP() {
  struct ip_info ip;
  wifi_get_ip_info(SOFTAP_IF, &ip);
  return IPAddress(ip.ip.addr | ~(ip.netmask.addr));
}

void PseudoDNS::onQueryTime() {
  IPAddress broadcast;
  if (WIFI_AP == WiFi.getMode()) {
    broadcast = softAPbroadcastIP();
  } else {
    broadcast = WiFi.broadcastIP();
  }
  Udp.beginPacket(broadcast, PORT);
  Udp.write(0x01);
  Udp.endPacket();
  //Serial.println("\n--->Send Query DNS package");
}

void PseudoDNS::onUdpDatagram(int packetSize) {
  byte buffer[packetSize];
  int len = Udp.read(buffer, sizeof(buffer));

  if (len > 0) {
    buffer[len] = 0;
    //Serial.println("<----Received packet from: " + Udp.remoteIP().toString() + " port: " + String(Udp.remotePort()) + " size: " + String(len) + " with data: " + String((char*)buffer));

    if (buffer[0] == 0x01) {
      if (mRunning) {
        byte responseBuffer[] = { 0x02 };
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(responseBuffer, sizeof(responseBuffer));
        Udp.write(mName.c_str(), mName.size());
        Udp.endPacket();
        //Serial.println("\n--->Send response package");
      }
    } else if (buffer[0] == 0x02) {
      if (mRunning) {

        std::string hostName = (char *)&buffer[1];
        std::string hostIP = Udp.remoteIP().toString().c_str();

        std::pair<std::string, std::string> hostNameAndIP(hostName, hostIP);

        if (dnsDiscoverdHosts.find(hostNameAndIP) == dnsDiscoverdHosts.end()) {
          dnsDiscoverdHosts.insert(hostNameAndIP);
          if (callbackFunction) {
            callbackFunction(hostName, hostIP);
          }
          if (callbackMethod) {
            callbackMethod(hostName, hostIP);
          }
        }
      }
    }
  }
}

void PseudoDNS::update() {

  if (mRunning || runQuery) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      onUdpDatagram(packetSize);
    }
  }

  //
  if (runQuery) {
    //Serial.print("PseudoDNS::update() 1");
    unsigned long currentTime = millis();
    if (currentTime - lastQueryTime >= QUERY_INTERVAL) {
      onQueryTime();
      lastQueryTime = currentTime;
    }
  }
}