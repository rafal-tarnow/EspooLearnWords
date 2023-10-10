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
    Q_PROPERTY(QString temperatureStr READ temperatureStr NOTIFY temperatureStrChanged)
    Q_PROPERTY(QString humidityStr READ humidityStr NOTIFY humidityStrChanged)
    Q_PROPERTY(QString pressureStr READ pressureStr NOTIFY pressureStrChanged)

public:
    Q_INVOKABLE float temperature();
    Q_INVOKABLE float humidity();
    Q_INVOKABLE float pressure();
    Q_INVOKABLE QString temperatureStr();
    Q_INVOKABLE QString humidityStr();
    Q_INVOKABLE QString pressureStr();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

signals:
    void temperatureChanged();
    void humidityChanged();
    void pressureChanged();
    void temperatureStrChanged();
    void humidityStrChanged();
    void pressureStrChanged();

private:

    float mTemp = -100.0f;
    float mPressure = -1000.0f;
    float mHumidity = -100.0f;
    QString mTempStr;
    QString mPressureStr;
    QString mHumidityStr;

    DBG_COUNT("T0002Controller");
};
