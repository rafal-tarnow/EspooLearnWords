#pragma once
#include <string>
#include "BrickClient.hpp"
#include "Debug.hpp"
#include "dtime.h"
#include "config.h"

class T0002 : public BrickClient
{
public:
  T0002(AsyncClient *asyncClient);
  T0002(TcpSocket *tcpSocket);
  void cmdSetMeasureTempPressHum(float temp, float pressure, float humidity);
  void cmdSetTestValue(uint64_t index, std::string text);
  static std::string type();
  void processProtocolStdFrame(std::deque<uint8_t> &frame) override;

private:
  DBG_COUNT("T0002");
};