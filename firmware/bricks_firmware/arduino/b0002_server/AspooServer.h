#pragma once

#include <string>

#include "Brick.h"
#include "PseudoDNS.h"
#include "B0002.h"
#include "TcpDevice.h"
#include "TcpDevicesServer.h"

class AspooServer {
  friend class Brick;
public:
  AspooServer(std::string name);
  void begin();
  void update();
private:
  void foundHost(const std::string &hostName, const std::string &hostIP);
  void onGetTcpDeviceNameAndType(TcpDevice *tcpDevice, const std::string &name, const std::string &type);
  void installTcpDeviceOnBrick(TcpDevice *tcpDevice, const std::string &name, const std::string &type);
  void uninstallTcpDeviceFromBrick(TcpDevice *tcpDevice);
  Brick* findBrick(const std::string &name, const std::string &type);
  Brick* findBrick(TcpDevice *tcpDevice);
  void onTcpDeviceConnected(TcpDevice *tcpDevice);
  void onTcpDeviceDisconnected(TcpDevice *tcpDevice);

  static void addBrick(Brick *brick);
  bool checkAllClientsConnected();

private:
  TcpDevicesServer tcpDevicesServer;
  static std::vector<Brick *> *mBricks;
  std::string mName;
  bool mAllClientsConnected = false;
  PseudoDNS pseudoDNS;
};