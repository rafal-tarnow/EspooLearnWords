#pragma once
#include "Application.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "B0002.hpp"

class ApplicationB0002 : public Application{
public:
ApplicationB0002();
void begin();
 void update() override;

private:
const int oneWirePin = 4;
B0002 mBrick;
OneWire oneWire;
DallasTemperature sensors;
};