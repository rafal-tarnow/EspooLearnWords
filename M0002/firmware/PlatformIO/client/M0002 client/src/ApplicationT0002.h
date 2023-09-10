#pragma once
#include <set>
#include "Application.h"
#include "T0002.hpp"
#include "dtime.h"
#include "Debug.hpp"


class ApplicationT0002 : public Application<T0002>
{
public:
    void loop() override;

private:
    DBG_COUNT("ApplicationT0002");
};