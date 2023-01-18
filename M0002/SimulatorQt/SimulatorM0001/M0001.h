#pragma once
#include <QObject>
#include <QUdpSocket>

class M0001 : public QObject{
    Q_OBJECT
public:
  M0001();
  ~M0001();

private:
  QUdpSocket *udpSocket = nullptr;
  void initSocket();
  void readPendingDatagrams();
};
