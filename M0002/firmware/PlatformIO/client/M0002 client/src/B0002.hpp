#pragma once
#include "BrickServer.hpp"

class B0002 : public BrickServer
{
public:
    void cmdSetMeasureTemp(float temp);
        std::string getBrickType() const override;
};