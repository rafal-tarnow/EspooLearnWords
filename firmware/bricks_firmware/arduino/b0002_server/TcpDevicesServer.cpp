#include "TcpDevicesServer.h"

TcpDevicesServer::TcpDevicesServer()
  : tcp_server(2883) {
}

void TcpDevicesServer::begin() {
  tcp_server.begin();
}

void TcpDevicesServer::update() {
  WiFiClient newClient = tcp_server.available();


  if (newClient) {
    Serial.println("New client connected");
    TcpDevice *tcpDevice = new TcpDevice();
    tcpDevice->wifiClient = newClient;
    clients.push_back(tcpDevice);
    tcpDevice->onTcpConnected();
    callbackDeviceConnected(tcpDevice);
  }

  // Obsługa klientów
  for (size_t i = 0; i < clients.size(); i++) {
    if (clients[i]->wifiClient.connected()) {
      size_t BUF_SIZE = 256;
      std::vector<uint8_t> dataVector;
      while (clients[i]->wifiClient.available()) {
        uint8_t buf[BUF_SIZE];
        int bytesRead = clients[i]->wifiClient.read(buf, BUF_SIZE);
        if (bytesRead > 0) {
          dataVector.insert(dataVector.end(), buf, buf + bytesRead);
        }
      }
      if (dataVector.size() > 0) {
        clients[i]->onTcpRawData(dataVector);
      }
    } else {
      Serial.print("Client ");
      Serial.print(i);
      Serial.println(" disconnected");
      callbackDeviceDisconnected(clients[i]);
      clients[i]->onTcpDisconnected();
      clients[i]->wifiClient.stop();
      delete clients[i];
      clients.erase(clients.begin() + i);
    }
  }
}