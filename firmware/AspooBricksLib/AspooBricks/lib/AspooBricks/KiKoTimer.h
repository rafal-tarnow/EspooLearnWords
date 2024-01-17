#pragma once
#include <cstdint>
#include <functional>
#include <set>

class KiKoTimer
{
    using CallbackTimeout = std::function<void()>;

public:
    void onTimeout(CallbackTimeout callback)
    {
        callbackTimeoutFunction = callback;
    }

    template <typename T>
    void onTimeout(T *obj, void (T::*method)())
    {
        callbackTimeoutMethod = [obj, method]()
        {
            (obj->*method)();
        };
    }
    void start(unsigned long msec);
    void start();
    void reset();
    void stop();

    static void update();
private:
    void mUpdate();

private:
    CallbackTimeout callbackTimeoutFunction;
    CallbackTimeout callbackTimeoutMethod;
    bool mRunning;
    unsigned long mLastTime = 0;
    unsigned long mInterval = 0;

    static std::set<KiKoTimer *> runningTimers;
    static std::set<KiKoTimer *> timersToInsert;
    static std::set<KiKoTimer *> timersToErase;

};