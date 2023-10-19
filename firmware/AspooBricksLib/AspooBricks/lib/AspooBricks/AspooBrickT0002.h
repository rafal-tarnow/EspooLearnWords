#pragma once
#include "AspooBricks.h"
#include <functional>

class AspooBrickT0002 : public AspooBrick
{
    using CallbackMeasure = std::function<void(float, float, float)>;

public:
    void onMeasure(CallbackMeasure callback)
    {
        callbackMeasure = callback;
    }

    template <typename T>
    void onMeasure(T *obj, void (T::*method)(float, float, float))
    {
        callbackMeasure = [obj, method](float temp, float pressure, float humidity)
        {
            (obj->*method)(temp, pressure, humidity);
        };
    }

private:
    CallbackMeasure callbackMeasure;
};