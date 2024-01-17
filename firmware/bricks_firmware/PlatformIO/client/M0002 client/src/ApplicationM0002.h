#pragma once
#include <set>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Application.h"
#include "M0002.hpp"

class ApplicationM0002 : public Application<M0002>
{
public:
  ApplicationM0002();
//   void begin();
    void setup() override;
   void loop() override;


 private:
   const int oneWirePin = 4;
   OneWire oneWire;
   DallasTemperature sensors;
};