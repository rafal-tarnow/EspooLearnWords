#pragma once
#include <ESP8266WiFi.h>
#include <functional>
#include <string>
#include <vector>
#include <deque>
#include <cstdint>
#include "ProtocolStd.h"

class TcpDevice {
  friend class TcpDevicesServer;
  using CallbackMethod = std::function<void(TcpDevice *, const std::string &, const std::string &)>;
  using CallbackMeasure = std::function<void(TcpDevice *, float temp)>;
  using CallbackProcotolFrame = std::function<void(std::deque<uint8_t> &)>;
public:
  TcpDevice();

  template<typename T>
  void onGetNameAndType(T *obj, void (T::*method)(TcpDevice *, const std::string &, const std::string &)) {
    callbackMethod = std::bind(method, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  }

  template<typename T>
  void onProtocolFrame(T *obj, void (T::*method)(std::deque<uint8_t> &)) {
    callbackProtocolFrame = [=](std::deque<uint8_t> &frame) {
      (obj->*method)(frame);
    };
  }

  void capi_getNameAndType();
  void capi_setMeasureInterval(bool measure, uint32_t interval_ms);
  void sendProtocolFrame(const std::vector<uint8_t> &frame);
  void sendProtocolFrame(std::deque<uint8_t> &frame);

private:
  void sapi_setNameAndType(std::string name, std::string type);
  void sapi_setTemperature(float temp);
  void onTcpRawData(std::vector<uint8_t> &rawData);
  void onProtocolStdFrame(std::deque<uint8_t> &frame);
  void onTcpConnected();
  void onTcpDisconnected();
  CallbackMethod callbackMethod;
  CallbackMeasure callbackMeasure;
  CallbackProcotolFrame callbackProtocolFrame;
  WiFiClient wifiClient;
  ProtocolStd protocolStd;
};