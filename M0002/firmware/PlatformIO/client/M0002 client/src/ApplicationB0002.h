#pragma once
#include <set>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Application.h"
#include "B0002.hpp"

class ApplicationB0002 : public Application<B0002>
{
public:
  ApplicationB0002();
//   void begin();
    void setup() override;
   void loop() override;


 private:
   const int oneWirePin = 4;
   OneWire oneWire;
   DallasTemperature sensors;
};