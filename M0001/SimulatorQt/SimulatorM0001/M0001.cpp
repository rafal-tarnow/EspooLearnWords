#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001() { initSocket(); }

M0001::~M0001() { delete udpSocket; }

void M0001::initSocket()
{
  udpSocket = new QUdpSocket(this);
  // udpSocket->bind(QHostAddress("192.168.122.209"), 7755);
  udpSocket->bind(45454, QUdpSocket::ShareAddress);

  connect(udpSocket, &QUdpSocket::readyRead, this, &M0001::readPendingDatagrams);
}

void M0001::readPendingDatagrams()
{
  static int i = 0;

  while (udpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    qDebug() << "data form socket " << i++;
    qDebug() << "destinationAdress=" << datagram.destinationAddress() << "destinationPort=" << datagram.destinationPort() << "senderAdress=" << datagram.senderAddress() << " senderPort=" << datagram.senderPort();
    // processTheDatagram(datagram);
  }
}
