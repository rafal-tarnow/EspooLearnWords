#pragma once

#include <QObject>
#include "Controller.hpp"
#include "./src/ObjectCounter.hpp"

class M0002Controller : public Controller
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)

public:
    Q_INVOKABLE float temperature();
    QString type() override;

signals:
    void temperatureChanged();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

private:
    float mTemp = 0.0f;

    DBG_COUNT("M0002Controller");
};