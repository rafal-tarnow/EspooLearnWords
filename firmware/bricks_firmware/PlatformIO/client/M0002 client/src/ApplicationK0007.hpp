#pragma once
#include <set>
#include <Bounce2.h>
#include "Application.h"
#include "K0007.hpp"

// single switch

class ApplicationK0007 : public Application<K0007>
{
public:
    ApplicationK0007();
    //   void begin();
    void setup() override;
    void loop() override;
    void handleNewBrickClient(K0007 * client) override;

private:
    int buttonChanges = 0;
    Bounce2::Button button;
    bool mSwitchState = false;
#define BUTTON_PIN 4
};