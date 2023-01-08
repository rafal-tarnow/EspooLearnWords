#pragma once
#include <QTimer>
#include <QUdpSocket>

class M0001 : public QObject {
  Q_OBJECT
public:
  typedef enum {
    TURN_OFF_LED,
    TURN_ON_LED,
  } ApiCommand;

  M0001(QString moduleName);
  ~M0001();
  void executeApiCommand(ApiCommand);

private slots:
  void readPendingDatagrams();

private:
  QString moduleName;
  QUdpSocket *socket = nullptr;
  QTimer *getIpAddresTimer = nullptr;
  void getDeviceIpAddress();
  void getIpAddressTimerEvent();
};
