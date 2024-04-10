#pragma once
#include "KiKoBrick.h"
#include <functional>

class AspooBrickT0002 : public KiKoBrick
{
    using CallbackMeasure = std::function<void(float, float, float)>;

public:
    void onMeasure(CallbackMeasure callback)
    {
        callbackMeasureFunction = callback;
    }

    template <typename T>
    void onMeasure(T *obj, void (T::*method)(float, float, float))
    {
        callbackMeasureMethod = [obj, method](float temp, float pressure, float humidity)
        {
            (obj->*method)(temp, pressure, humidity);
        };
    }

protected:
    void handleFrame(uint8_t functionCode, std::deque<uint8_t> &frame) override;

private:
    CallbackMeasure callbackMeasureFunction;
    CallbackMeasure callbackMeasureMethod;
};