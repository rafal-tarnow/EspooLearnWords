#include "KiKoTimer.h"
#include <Arduino.h>

std::set<KiKoTimer *> KiKoTimer::runningTimers;
std::set<KiKoTimer *> KiKoTimer::timersToInsert;
std::set<KiKoTimer *> KiKoTimer::timersToErase;

void KiKoTimer::start(unsigned long msec)
{
    mInterval = msec;
    start();
}

void KiKoTimer::start()
{
    mLastTime = millis();
    mRunning = true;
    timersToInsert.insert(this);
}

void KiKoTimer::reset()
{
    mLastTime = millis();
}

void KiKoTimer::stop()
{
    mRunning = false;
    timersToErase.insert(this);
}

void KiKoTimer::update()
{
    if (timersToErase.size() > 0)
    {
        for (auto timer : timersToErase)
        {
            runningTimers.erase(timer);
        }
        timersToErase.clear();
    }

    if (timersToInsert.size() > 0)
    {
        for (auto timer : timersToInsert)
        {
            runningTimers.insert(timer);
        }
        timersToInsert.clear();
    }

    if (runningTimers.size() > 0)
    {
        for (auto timer : runningTimers)
        {
            timer->mUpdate();
        }
    }
}

void KiKoTimer::mUpdate()
{
    if (mRunning)
    {
        if (millis() >= (mLastTime + mInterval))
        {
            mLastTime = millis();
            if (callbackTimeoutFunction)
            {
                callbackTimeoutFunction();
            }
            if (callbackTimeoutMethod)
            {
                callbackTimeoutMethod();
            }
        }
    }
}
