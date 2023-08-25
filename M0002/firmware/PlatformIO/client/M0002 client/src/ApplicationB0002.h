#pragma once
#include "Application.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "B0002.hpp"

class ApplicationB0002 : public Application
{
public:
  ApplicationB0002();
  void begin();
  void update() override;
  std::string getBrickType() const override;
  BrickClient *createBrickClient(AsyncClient *) override;
  void deleteBrickClient(BrickClient *) override;

private:
std::set<B0002*> clients;
  const int oneWirePin = 4;
  OneWire oneWire;
  DallasTemperature sensors;
};