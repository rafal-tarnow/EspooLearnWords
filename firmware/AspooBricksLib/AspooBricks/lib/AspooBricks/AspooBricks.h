#pragma once

#include <string>
#include <functional>

#include <PseudoDNS.h>

class AspooBrick
{
    using CallbackConnection = std::function<void(bool)>;

public:
    void onConnection(CallbackConnection callback)
    {
        callbackConnection = callback;
    }

    template <typename T>
    void onConnection(T *obj, void (T::*method)(bool))
    {
        callbackConnection = [obj, method](bool connected)
        {
            (obj->*method)(connected);
        };
    }

    void setId(std::string);
    void begin();
    static void update();

private:
    std::string mId;
    static PseudoDNS pseudoDNS;
    CallbackConnection callbackConnection;
};

#include "AspooBrickT0002.h"




