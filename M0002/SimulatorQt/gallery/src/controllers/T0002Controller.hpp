#pragma once

#include <QObject>
#include "Controller.hpp"
#include "../ObjectCounter.hpp"

class T0002Controller : public Controller
{
    Q_OBJECT
    DBG_COUNT("T0002Controller");
};
