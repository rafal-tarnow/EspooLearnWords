#include "TimerMicros.hpp"
#include <Arduino.h>

std::list<TimerMicros *> TimerMicros::timers;

TimerMicros::TimerMicros()
{
}

TimerMicros::~TimerMicros()
{
    timers.remove(this);
}

void TimerMicros::startMicros(uint64_t micros)
{
    mPeriodMicros = micros;
    mActive = true;
    mLastMicros = micros64();
    timers.push_back(this);
}

void TimerMicros::setSingleShot(bool singleShot)
{
    mSingleShot = singleShot;
}

void TimerMicros::stop()
{
    mActive = false;
}

void TimerMicros::update()
{
    uint64_t currentMicros = micros64();

    for (auto it = timers.begin(); it != timers.end();)
    {
        if ((*it)->mActive)
        {
            if (currentMicros - (*it)->mLastMicros >= (*it)->mPeriodMicros)
            {
                (*it)->mLastMicros = currentMicros;

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