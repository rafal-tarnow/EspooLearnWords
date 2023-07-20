#include "PseudoDNS.h"

void PseudoDNS::begin(){
  Udp.begin(1234);
}

void PseudoDNS::startServer(std::string name) {
  mName = name;
  mRunning = true;
}

void PseudoDNS::stopServer() {
  mRunning = false;
}

bool PseudoDNS::isRunning(){
  return mRunning;
}

void PseudoDNS::sendPseudoDNSPackage() {
    Udp.beginPacket(broadcastIP, 1234);
    Udp.write(mName.c_str());
    Udp.endPacket();
}

void PseudoDNS::loop() {
      if (mRunning) {
        unsigned long currentTime = millis();
        if (currentTime - lastTime >= interval) {
            sendPseudoDNSPackage();
            lastTime = currentTime;
        }
    }
}