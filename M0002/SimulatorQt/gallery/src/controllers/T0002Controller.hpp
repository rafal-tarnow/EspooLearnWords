#pragma once

#include <QObject>
#include "Controller.hpp"
#include "../ObjectCounter.hpp"

class T0002Controller : public Controller
{
    Q_OBJECT
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY humidityChanged)
    Q_PROPERTY(float pressure READ pressure NOTIFY pressureChanged)

public:
    Q_INVOKABLE float temperature();
    Q_INVOKABLE float humidity();
    Q_INVOKABLE float pressure();

signals:
    void temperatureChanged();
    void humidityChanged();
    void pressureChanged();

private:

    float mTemp = -100.0f;
    float mPressure = -1000.0f;
    float mHumidity = -100.0f;

    DBG_COUNT("T0002Controller");
};
