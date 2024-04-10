#pragma once

#include <cstdint>
#include <list>
#include <functional>

class Timer
{
    using Callback = std::function<void()>;

public:
    Timer();
    ~Timer();
    void start(uint64_t milis);
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
    static std::list<Timer *> timers;
    Callback callbackMethod;
    Callback callbackFunction;
    bool mActive = false;
    bool mSingleShot = false;
    uint64_t mPeriod = 0;
    uint64_t mLastMillis = 0;
};