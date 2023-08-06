#include "AspooServer.h"

std::vector<Brick *>* AspooServer::mBricks = nullptr;

AspooServer::AspooServer(std::string name) {
  mName = name;
}

void AspooServer::begin() {

  pseudoDNS.run(mName);
  pseudoDNS.onHostFound(this, &AspooServer::foundHost);
  pseudoDNS.startQueriesForAllHosts();

  tcpDevicesServer.begin();
  tcpDevicesServer.onDeviceConnected(this, &AspooServer::onTcpDeviceConnected);
  tcpDevicesServer.onDeviceDisconnected(this, &AspooServer::onTcpDeviceDisconnected);
}

void AspooServer::addBrick(Brick *brick) {
  if(mBricks == nullptr){
    mBricks = new std::vector<Brick *>();
  }
  mBricks->push_back(brick);
}

bool AspooServer::checkAllClientsConnected() {
  for (Brick *brick : *mBricks) {
    if (!brick->isConnected()) {
      return false;
    }
  }
  return true;
}

void AspooServer::foundHost(const std::string &hostName, const std::string &hostIP) {
  Serial.print("AspooServer::foundHost: ");
  Serial.print(hostName.c_str());
  Serial.print(" ");
  Serial.println(hostIP.c_str());
}

void AspooServer::onTcpDeviceConnected(TcpDevice *tcpDevice) {
  Serial.print("AspooServer::tcpDeviceConnected()");
  tcpDevice->onGetNameAndType(this, &AspooServer::onGetTcpDeviceNameAndType);
  tcpDevice->capi_getNameAndType();
}

void AspooServer::onGetTcpDeviceNameAndType(TcpDevice *tcpDevice, const std::string &name, const std::string &type) {
    Serial.print("AspooServer::onGetTcpDeviceNameAndType()");
  Serial.print(name.c_str());
  Serial.print(" ");
  Serial.println(type.c_str());
  installTcpDeviceOnBrick(tcpDevice, name, type);
}

void AspooServer::installTcpDeviceOnBrick(TcpDevice *tcpDevice, const std::string &name, const std::string &type) {
  Serial.print("AspooServer::installTcpDeviceOnBrick()");
  Serial.print(name.c_str());
  Serial.print(" ");
  Serial.println(type.c_str());

  Brick* brick = findBrick(name, type);
  if(brick != nullptr){
      Serial.print("AspooServer::installTcpDeviceOnBrick() install");
      brick->installTcpDevice(tcpDevice);
  }
}

Brick *AspooServer::findBrick(const std::string &name, const std::string &type) {
  Serial.println("AspooServer::findBrick()");
  for (const auto &brick : *mBricks) {
    if (brick->getName() == name && brick->getType() == type) {
      return brick;  
    }
  }
  return nullptr;
}

Brick *AspooServer::findBrick(TcpDevice *tcpDevice) {
  for (const auto &brick : *mBricks) {
    if (brick->mTcpDevice == tcpDevice) {
      return brick;  
    }
  }
  return nullptr;
}

void AspooServer::onTcpDeviceDisconnected(TcpDevice *tcpDevice) {
  Serial.print("AspooServer::tcpDeviceDisconnected()");
  uninstallTcpDeviceFromBrick(tcpDevice);
}

void AspooServer::uninstallTcpDeviceFromBrick(TcpDevice *tcpDevice) {
  Serial.print("AspooServer::uninstallTcpDeviceFromBrick()");

  Brick* brick = findBrick(tcpDevice);
  if(brick != nullptr){
      brick->uninstallTcpDevice(tcpDevice);
  }
}

void AspooServer::update() {
  pseudoDNS.update();
  tcpDevicesServer.update();
}