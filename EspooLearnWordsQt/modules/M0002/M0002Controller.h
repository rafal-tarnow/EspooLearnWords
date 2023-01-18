#pragma once
#include "M0002.h"
#include <QObject>

class M0002Controller : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool turnOnLed WRITE setTurnOn)
    Q_PROPERTY(QString deviceName READ getDeviceName WRITE setDeviceName NOTIFY deviceNameChanged)
public:
    M0002Controller(QObject *parent = nullptr);
    ~M0002Controller();

    Q_INVOKABLE void initialize(QString deviceName);
    Q_INVOKABLE void setNetworkConfiguration(bool ap_mode);

    void setTurnOn(const bool &search);
    Q_INVOKABLE bool getTurnOn() const;
    Q_INVOKABLE bool getAPMode() const;

    void setDeviceName(const QString &search);
    QString getDeviceName() const;

Q_SIGNALS:
    void deviceNameChanged();
    void deviceInited();

private slots:
    void deviceInitedEvent();
private:
    M0002 *module = nullptr;
    QString deviceName;
};
