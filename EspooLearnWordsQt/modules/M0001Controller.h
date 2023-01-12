#pragma once
#include "M0001.h"
#include <QObject>

class M0001Controller : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool turnOnLed READ getTurnOn WRITE setTurnOn NOTIFY turnOnChanged)
  Q_PROPERTY(QString deviceName READ getDeviceName WRITE setDeviceName NOTIFY deviceNameChanged)
public:
  M0001Controller(QObject *parent = nullptr);
  ~M0001Controller();

  Q_INVOKABLE void initialize(QString deviceName);
  Q_INVOKABLE void setNetworkConfiguration(bool ap_mode);

  void setTurnOn(const bool &search);
  bool getTurnOn() const;

  void setDeviceName(const QString &search);
  QString getDeviceName() const;

Q_SIGNALS:
  void turnOnChanged();
  void deviceNameChanged();

private:
  bool turnOnState = false;
  M0001 *module = nullptr;
  QString deviceName;
};
