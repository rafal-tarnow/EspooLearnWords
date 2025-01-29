#pragma once

#include <QObject>
#include <QLoggingCategory>
#include "Controller.hpp"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(K0004ControllerClass)

class K0004Controller : public Controller
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool relayState READ relayState WRITE setRelayState NOTIFY relayStateChanged)
public:
    explicit K0004Controller(QObject *parent = nullptr, QString id = "", QString name = "");
    Q_INVOKABLE bool relayState();
    Q_INVOKABLE void setRelayState(bool newState);
    QString type() override;

signals:
    void relayStateChanged();

protected:
    uint8_t handleProtocolFrame(QByteArray & frame) override;

private:
    void cmdSetRelayState();

private:
    float mTemp = 0.0f;
    bool mRelayState = false;
    std::unique_ptr<QTimer> relayTimer;

    DBG_COUNT("K0004Controller");
};
