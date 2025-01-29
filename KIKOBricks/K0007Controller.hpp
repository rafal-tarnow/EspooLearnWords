#pragma once

#include <QObject>
#include <QLoggingCategory>
#include "Controller.hpp"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(K0007ControllerClass)

class K0007Controller : public Controller
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool switchState READ switchState NOTIFY switchStateChanged)
public:
    explicit K0007Controller(QObject *parent = nullptr, QString id = "", QString name = "");
    Q_INVOKABLE bool switchState();
    QString type() override;

signals:
    void switchStateChanged();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

private:
    bool mSwitchState = false;
    int mStatesChanges = 0;

    DBG_COUNT("K0007Controller");
};
