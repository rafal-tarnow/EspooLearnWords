#pragma once
#include <ESP8266WiFi.h>
#include <vector>
#include <functional>
#include "TcpDevice.h"

class TcpDevicesServer {
  using CallbackMethod = std::function<void(TcpDevice*)>;
public:
  TcpDevicesServer();
  void begin();

  template<typename T>
  void onDeviceConnected(T* obj, void (T::*method)(TcpDevice*)) {
    callbackDeviceConnected = [=](TcpDevice* device) {
      (obj->*method)(device);
    };
  }
  template<typename T>
  void onDeviceDisconnected(T* obj, void (T::*method)(TcpDevice*)) {
    callbackDeviceDisconnected = [=](TcpDevice* device) {
      (obj->*method)(device);
    };
  }
  void update();
private:
  CallbackMethod callbackDeviceConnected;
  CallbackMethod callbackDeviceDisconnected;
  std::vector<TcpDevice *> clients;
  WiFiServer tcp_server;
};