// #include "device.h"
// #include "diode.h"

// char deviceNameReply[] = "Ledy biurko";

// void Device::setup() {
//   // Begin listening to UDP port
//   UDP.begin(UDP_PORT);
//   Serial.print("Listening on UDP port ");
//   Serial.println(UDP_PORT);
// }

// void Device::loop() {

//   // If packet received...
//   int packetSize = UDP.parsePacket();
//   if (packetSize) {
//     // Serial.print("Received packet ");
//     // Serial.print(packageIndex++);
//     // Serial.print("! Size: ");
//     // Serial.println(packetSize);
//     int len = UDP.read(packet, 255);
//     if (len > 0) {
//       if (packet[0] == 1) {
//         parseEchoCommand(packet);
//       }
//       if (packet[0] == 2) {
//         Serial.println("Status command");
//         parseStatusCommand(packet);
//       }
//       if (packet[0] == 3) {
//         Serial.println("Set network config command");
//         parseNetworkConfigCommand(packet);
//       }
//       if (packet[0] == 4) {
//         Serial.println("Get full device status command");
//         parseGetFullDeviceStatusCommand(packet);
//       }
//     }
//   }
//   // Serial.print("Packet received: ");
//   // Serial.println(packet);

//   // Send return packet
// }

// void Device::parseEchoCommand(char *packet) {
//   Serial.println("Echo command");
//   UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
//   UDP.write(0x01);
//   UDP.write(deviceNameReply);
//   UDP.endPacket();
// }

// void Device::parseGetFullDeviceStatusCommand(char *packet) {
//   char deviceStatus[2];
//   deviceStatus[0] = 4;
//   deviceStatus[1] = 0x00;

//   UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());

//   bool onOff = getOnOffState();
//   if (onOff) {
//     deviceStatus[1] |= 0b00000001;
//   }

//   preferences.begin("M0001", true);
//   bool apMode = preferences.getBool("APMode", 0);
//   preferences.end();
  
//   if (apMode) {
//     deviceStatus[1] |= 0b00000010;
//   }
//    Serial.println("GetAP Mode");
//       Serial.println(apMode);


//   UDP.write(deviceStatus, 2);
//   UDP.endPacket();
// }

// void Device::parseNetworkConfigCommand(char *packet) {
//   preferences.begin("M0001", false);
//   if (packet[1] & 0b00000001) {
//     Serial.println("Network AP mode");
//     preferences.putBool("APMode", 1);
//   } else {
//     Serial.println("Network WiFi mode");
//     preferences.putBool("APMode", 0);
//   }
//   preferences.end();
// }

// void Device::parseStatusCommand(char *packet) {
//   if (packet[1] & 0b00000001) {
//     setOnOff(true);
//   } else {
//     setOnOff(false);
//   }
// }