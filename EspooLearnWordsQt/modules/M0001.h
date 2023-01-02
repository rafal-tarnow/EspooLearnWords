#pragma once
#include <QTimer>
#include <QUdpSocket>

class M0001 : public QObject {
  Q_OBJECT
  typedef enum {
    TURN_ON_LED,
    TURN_OFF_LED,
  } ApiCommand;

public:
  M0001();
  ~M0001();
  void executeApiCommand(ApiCommand);

private slots:
  void sendTimerEvent();
private:
  QUdpSocket *udpSocket = nullptr;
  QTimer *sendTimer = nullptr;
  void initSocket();
  void readPendingDatagrams();
};
