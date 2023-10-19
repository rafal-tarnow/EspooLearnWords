#pragma once

#include <string>
#include "TcpDevice.h"

class Brick {
  friend class AspooServer;
  using ConnectionCallback = std::function<void()>;
public:
  Brick(std::string name);
  std::string getName();
  virtual std::string getType() const = 0;

  void onConnected(ConnectionCallback callback) {
    callbackConnFunc = callback;
  }
  template<typename T>
  void onConnected(T *obj, void (T::*method)()) {
    callbackConnMeth = [=]() {
      (obj->*method)();
    };
  }
  void onDisconnected(ConnectionCallback callback) {
    callbackDisconnFunc = callback;
  }
  template<typename T>
  void onDisconnected(T *obj, void (T::*method)()) {
    callbackDisconnMeth = [=]() {
      (obj->*method)();
    };
  }
  bool isConnected();

protected:
  TcpDevice *mTcpDevice = nullptr;
  virtual void onProtocolFrame(std::deque<uint8_t> &) = 0;
private:
  ConnectionCallback callbackConnFunc;
  ConnectionCallback callbackConnMeth;
  ConnectionCallback callbackDisconnFunc;
  ConnectionCallback callbackDisconnMeth;
  void installTcpDevice(TcpDevice *);
  void uninstallTcpDevice(TcpDevice* device);
  std::string mName;
  bool mIsConnected = false;
};