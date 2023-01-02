#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001() { initSocket(); }

M0001::~M0001() { delete udpSocket; }

void M0001::initSocket()
{
  udpSocket = new QUdpSocket(this);
  udpSocket->bind(QHostAddress::Any, 7755);

  connect(udpSocket, &QUdpSocket::readyRead, this, &M0001::readPendingDatagrams);
}

void M0001::readPendingDatagrams()
{
  qDebug() << "data form socket";
  while (udpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    // processTheDatagram(datagram);
  }
}
