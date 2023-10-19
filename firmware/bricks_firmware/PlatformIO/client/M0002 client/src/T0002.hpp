#pragma once
#include <string>
#include "BrickClient.hpp"
#include "Debug.hpp"
#include "dtime.h"

class T0002 : public BrickClient
{
public:
  T0002(AsyncClient *asyncClient);
  T0002(TcpSocket *tcpSocket);
  void cmdSetMeasureTempHumPress(float temp, float humidity, float pressure);
  void cmdSetTestValue(uint64_t index, std::string text);
  static std::string type();
private:
  DBG_COUNT("T0002");
};