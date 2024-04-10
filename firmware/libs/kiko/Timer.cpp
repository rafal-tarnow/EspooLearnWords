#include "Timer.hpp"
#include <Arduino.h>

std::list<Timer *> Timer::timers;

Timer::Timer()
{
}

Timer::~Timer()
{
    timers.remove(this);
}

void Timer::start(uint64_t milis)
{
    mPeriod = milis;
    mActive = true;
    mLastMillis = millis();
    timers.push_back(this);
}

void Timer::setSingleShot(bool singleShot)
{
    mSingleShot = singleShot;
}

void Timer::stop()
{
    mActive = false;
}

void Timer::update()
{
    uint64_t currentMillis = millis();

    for (auto it = timers.begin(); it != timers.end();)
    {
        if ((*it)->mActive)
        {
            if (currentMillis - (*it)->mLastMillis >= (*it)->mPeriod)
            {
                (*it)->mLastMillis = currentMillis;

                if ((*it)->callbackFunction)
                    (*it)->callbackFunction();
                if ((*it)->callbackMethod)
                    (*it)->callbackMethod();

                if ((*it)->mSingleShot)
                    (*it)->mActive = false;
            }
            ++it;
        }
        else
        {
            it = timers.erase(it);
        }
    }
}