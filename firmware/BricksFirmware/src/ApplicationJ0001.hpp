#pragma once

#include <set>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Application.h"
#include "J0001Client.hpp"
#include "config.h"

class ApplicationJ0001 : public Application<J0001Client>
{
    struct BrickState
    {
        bool m_run = false;
        float m_dropletVolume = 0.05f;
        uint8_t m_windowDuration = 5;
        float m_flowRate = 80.0f;
        float maxFlowDisplay = 80.0f;
    };

public:
    ApplicationJ0001();
    //   void begin();
    void setup() override;
    void loop() override;
    void handleNewBrickClient(J0001Client *client) override;
    void handleRunState(J0001Client *clien, bool run);
    void handleDropletVolume(J0001Client *client, float dropletVolume);
    void handleWindowDuration(J0001Client *client, uint8_t windowDuration);
    void handleResetMeasurement(J0001Client *client);
    void handleMaxFlowDisplay(J0001Client *client, float maxFlowDisplay);

private:
float milliliters();

private:
    BrickState brickState;
    const int inputPin = 4;

    DBG_COUNT("ApplicationJ0001");
};