#pragma once

#include "AspooServer.h"

class Application{
public:
  void onConnectedTempBoiler();
  void onDisconnectedTempBoiler();
  void onnMeasureBoiler(float temp);
};