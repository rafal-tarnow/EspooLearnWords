#pragma once
#include <set>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Application.h"
#include "M0004.hpp"

class ApplicationM0004 : public Application<M0004>
{
public:
  ApplicationM0004();
//   void begin();
    void setup() override;
   void loop() override;


 private:
   const int oneWirePin = 4;
   OneWire oneWire;
   DallasTemperature sensors;
};