#pragma once

#include <QObject>
#include "Controller.hpp"
#include "../ObjectCounter.hpp"

class M0002Controller : public Controller
{
    Q_OBJECT
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)

public:
    Q_INVOKABLE float temperature();

signals:
    void temperatureChanged();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

private:
    float mTemp = 0.0f;

    DBG_COUNT("M0002Controller");
};
