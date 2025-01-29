#pragma once

#include <cstdint>
#include <list>
#include <functional>



/*
Example2:     
    timer.setSingleShot(true);
    timer.onTimeout(this, &ApplicationStateManager::handleTransitionToRunTimeout);
*/


class TimerMicros
{
    using Callback = std::function<void()>;

public:
    TimerMicros();
    ~TimerMicros();
    void startMicros(uint64_t micros);
    void setSingleShot(bool singleShot);
    void stop();

    void onTimeout(Callback callback)
    {
        callbackFunction = callback;
    }

    template <typename T>
    void onTimeout(T *obj, void (T::*method)())
    {
        callbackMethod = std::bind(method, obj);
    }

    static void update();

private:
    static std::list<TimerMicros *> timers;
    Callback callbackMethod;
    Callback callbackFunction;
    bool mActive = false;
    bool mSingleShot = false;
    uint64_t mPeriodMicros = 0;
    uint64_t mLastMicros = 0;
};