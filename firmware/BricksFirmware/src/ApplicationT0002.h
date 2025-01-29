#pragma once
#include <set>
#include "Application.h"
#include "T0002.hpp"
#include "dtime.h"
#include "Debug.hpp"
#include "TimerMicros.hpp"

class ApplicationT0002 : public Application<T0002>
{
    struct BrickState
    {
        float frequency = 100;
    };

public:
    void setup() override;
    void loop() override;
    void handleNewBrickClient(T0002 *client) override;
    void handleFrequency(T0002 *client, float freq);

    void handlePWMTimer();

private:
uint64_t calculatePeriod(float freq);

private:
    TimerMicros timerPWM;
    BrickState brickState;

    DBG_COUNT("ApplicationT0002");
};