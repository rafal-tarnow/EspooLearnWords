#pragma once
#include <set>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Application.h"
#include "M0004.hpp"
#include "Timer.hpp"

// single relay

class ApplicationK0004 : public Application<M0004>
{
public:
  ApplicationK0004();
  //   void begin();
  void setup() override;
  void loop() override;
  void handleNewBrickClient(M0004 * client) override;
  void handleRelayState(M0004 * client, bool state);
  void handleRelayTimer();

private:
#define RELAY_PIN 4
Timer timer;
bool mRelayState = false;
};