#pragma once

#include <QObject>
#include "Controller.hpp"
#include "./src/ObjectCounter.hpp"

class K0002Controller : public Controller
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)

public:
    explicit K0002Controller(QObject *parent = nullptr, QString id = "", QString name = "");
    Q_INVOKABLE float temperature();
    QString type() override;

signals:
    void temperatureChanged();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

private:
    float mTemp = 0.0f;

    DBG_COUNT("K0002Controller");
};
