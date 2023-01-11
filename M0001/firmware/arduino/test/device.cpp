#include "device.h"
#include "diode.h"

char deviceNameReply[] = "Ledy biurko";

void Device::setup() {
  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
}

void Device::loop() {
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
      if (packet[0] == 1) {
        Serial.println("Echo command");
        UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        //UDP.write(packet,4);
        UDP.write(deviceNameReply);
        UDP.endPacket();
      }
      if (packet[0] == 2) {
        Serial.println("Status command");
        parseStatusCommand(packet);
      }
    }
  }
  // Serial.print("Packet received: ");
  // Serial.println(packet);

  // Send return packet
}

void Device::parseStatusCommand(char *packet) {
  Serial.print("    packet[1] = ");
  Serial.println(int(packet[1]));
  if (packet[1] & 0b00000001) {
    LED(true);
  } else {
    LED(false);
  }
}