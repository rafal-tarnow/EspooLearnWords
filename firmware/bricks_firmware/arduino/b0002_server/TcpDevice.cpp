#include "TcpDevice.h"

TcpDevice::TcpDevice() {
  protocolStd.setOnFrameCallback(this, &TcpDevice::onProtocolStdFrame);
}

void TcpDevice::capi_getNameAndType() {
  Serial.println("TcpDevice::getNameAndType");
  std::vector<uint8_t> package;
  package.push_back(0x02);
  sendProtocolFrame(package);
}

void TcpDevice::sendProtocolFrame(const std::vector<uint8_t>& frame) {
  if (wifiClient.connected()) {
    uint16_t size = frame.size();

    std::vector<uint8_t> datagram;
    ProtocolStd::append(datagram, size);
    datagram.insert(datagram.end(), frame.begin(), frame.end());

    wifiClient.write(datagram.data(), datagram.size());
  }
}

void TcpDevice::sendProtocolFrame(std::deque<uint8_t>& frame) {
  if (wifiClient.connected()) {
    uint16_t size = frame.size();
    ProtocolStd::prepend(frame, size);

    std::vector<uint8_t> dataArray(frame.begin(), frame.end());
    wifiClient.write(dataArray.data(), dataArray.size());
  }
}

void TcpDevice::onTcpConnected() {
  Serial.println("TcpDevice::onTcpConnected()");
  protocolStd.reset();
}

void TcpDevice::onTcpDisconnected() {
  Serial.println("TcpDevice::onTcpDisconnected()");
  protocolStd.reset();
}

void TcpDevice::onProtocolStdFrame(std::deque<uint8_t>& frame) {
  Serial.println("TcpDevice::onProtocolStdFrame()");

  if (!frame.empty()) {
    uint8_t functionCode = frame.front();
    if (functionCode == 0x02) {
      uint8_t functionCode = ProtocolStd::getUint8_t(frame);
      std::string deviceName = ProtocolStd::getString(frame);
      std::string deviceType = ProtocolStd::getString(frame);
      sapi_setNameAndType(deviceName, deviceType);
    } else {
      if (callbackProtocolFrame) {
        callbackProtocolFrame(frame);
      }
    }
  }
}

void TcpDevice::sapi_setNameAndType(std::string deviceName, std::string deviceType) {
  if (callbackMethod) {
    callbackMethod(this, deviceName, deviceType);
  }
}

void TcpDevice::sapi_setTemperature(float temp) {
  if (callbackMeasure) {
    callbackMeasure(this, temp);
  }
}

void TcpDevice::onTcpRawData(std::vector<uint8_t>& rawData) {
  Serial.println("TcpDevice::onTcpRawData()");
  for (size_t i = 0; i < rawData.size(); i++) {
    // Wydrukuj każdy bajt w formacie "0xXX" na Serial
    Serial.print("0x");
    if (rawData[i] < 0x10) {
      // Dodaj zero przed jednocyfrowymi wartościami, aby zachować spójność
      Serial.print("0");
    }
    Serial.print(rawData[i], HEX);

    // Dodaj spacje między bajtami, z wyjątkiem ostatniego bajtu
    if (i < rawData.size() - 1) {
      Serial.print(" ");
    }
  }

  // Nowa linia po wypisaniu całego wektora
  Serial.println();

  protocolStd.addData(rawData);
}