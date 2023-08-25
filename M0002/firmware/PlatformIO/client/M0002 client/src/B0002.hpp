#pragma once
#include "BrickServer.hpp"

class B0002 : public BrickClient
{
public:
    B0002(AsyncClient *asyncClient);
    void cmdSetMeasureTemp(float temp);
      
};