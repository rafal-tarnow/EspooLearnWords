#pragma once

#include "AspooServer.h"

class Application{
public:
  void onMeasureBoilerTemp(AspooClient * client, float temp);
};