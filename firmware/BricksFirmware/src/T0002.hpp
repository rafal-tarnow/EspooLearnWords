#pragma once
#include <string>
#include "BrickClient.hpp"
#include "Debug.hpp"
#include "dtime.h"
#include "config.h"

class T0002 : public BrickClient
{
  using CallbackSetFloat = std::function<void(T0002 *, float)>;

public:
  T0002(AsyncClient *asyncClient);
  T0002(TcpSocket *tcpSocket);
  void cmdSetMeasureTempPressHum(float temp, float pressure, float humidity);
  void cmdSetTestValue(uint64_t index, std::string text);
  void cmdSetFrequency(float freq);
  static std::string type();
  void processProtocolStdFrame(std::deque<uint8_t> &frame) override;

  template <typename T>
  void onSetFrequency(T *obj, void (T::*method)(T0002 *, float))
  {
    callbackSetFrequency = [obj, method](T0002 *client, float freq)
    {
      (obj->*method)(client, freq);
    };
  }

private:
  CallbackSetFloat callbackSetFrequency;
  DBG_COUNT("T0002");
};