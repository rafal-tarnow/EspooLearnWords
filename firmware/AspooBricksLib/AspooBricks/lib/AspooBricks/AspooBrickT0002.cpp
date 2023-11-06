#include "AspooBrickT0002.h"

void AspooBrickT0002::handleFrame(uint8_t functionCode, std::deque<uint8_t> &frame)
{
    float temperature = ProtocolStd::getFloat(frame);
    float pressure = ProtocolStd::getFloat(frame);
    float humidity = ProtocolStd::getFloat(frame);

    if (callbackMeasureFunction)
    {
        callbackMeasureFunction(temperature, pressure, humidity);
    }

    if (callbackMeasureMethod)
    {
        callbackMeasureMethod(temperature, pressure, humidity);
    }
}