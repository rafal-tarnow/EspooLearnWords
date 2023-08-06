#include "Brick.h"
#include "AspooServer.h"

Brick::Brick(std::string name) {
  Serial.println("Brick::Brick()");
  mName = name;
  AspooServer::addBrick(this);
}

std::string Brick::getName() {
  return mName;
}

bool Brick::isConnected() {
  return mTcpDevice ? true : false;
}

void Brick::installTcpDevice(TcpDevice* device) {
  Serial.println("Brick::installTcpDevice");
  if (device) {
    mTcpDevice = device;
    mTcpDevice->onProtocolFrame(this, &Brick::onProtocolFrame);
    if (callbackConnFunc) {
      callbackConnFunc();
    }
    if (callbackConnMeth) {
      callbackConnMeth();
    }
  }
}

void Brick::uninstallTcpDevice(TcpDevice* device) {
  Serial.println("Brick::installTcpDevice");
  if (device) {
    if (callbackDisconnMeth) {
      callbackDisconnMeth();
    }
    if (callbackDisconnFunc) {
      callbackDisconnFunc();
    }
    mTcpDevice = nullptr;
  }
}