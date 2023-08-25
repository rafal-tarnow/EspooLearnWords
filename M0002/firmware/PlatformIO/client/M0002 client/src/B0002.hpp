#pragma once
#include "BrickServer.hpp"
#include "Debug.hpp"

class B0002 : public BrickClient
{
public:
    B0002(AsyncClient *asyncClient);
    void cmdSetMeasureTemp(float temp);
private:
DBG_COUNT("B0002");
};