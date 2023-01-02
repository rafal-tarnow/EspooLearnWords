#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001()
{
  initSocket();
  sendTimer = new QTimer(this);
  connect(sendTimer, &QTimer::timeout, this, &M0001::sendTimerEvent);
  sendTimer->start(1000);
}

M0001::~M0001()
{
  delete sendTimer;
  delete udpSocket;
}

void M0001::sendTimerEvent()
{
  QByteArray data;
  data.append(0x01);
  data.append(0x02);
  data.append(0x03);
  data.append(0x04);
  data.append(0x05);

  qDebug() << "Timer event = " << udpSocket->writeDatagram(data, QHostAddress("192.168.146.209"), 7755);
}

void M0001::executeApiCommand(ApiCommand) {}

void M0001::initSocket()
{
  udpSocket = new QUdpSocket(this);
  udpSocket->bind(QHostAddress::LocalHost, 7755);

  connect(udpSocket, &QUdpSocket::readyRead, this, &M0001::readPendingDatagrams);
}

void M0001::readPendingDatagrams()
{
  while (udpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    // processTheDatagram(datagram);
  }
}
