#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001()
{
  initSocket();
  sendTimer = new QTimer(this);
  connect(sendTimer, &QTimer::timeout, this, &M0001::sendTimerEvent);
  sendTimer->start(5000);
}

M0001::~M0001()
{
  delete sendTimer;
  delete udpSocket;
}

void M0001::sendTimerEvent()
{
  static uint32_t value = 0;

  union {
    char dataa[4];
    uint32_t val;
  } Frame;
  Frame.val = value;
  value++;
  QByteArray data;
  data.append(Frame.dataa, 4);

  // qDebug() << "Timer event = " << udpSocket->writeDatagram(data, QHostAddress("192.168.122.209"), 7755);
  qDebug() << "Timer event = " << udpSocket->writeDatagram(data, QHostAddress::Broadcast, 45455);
}

void M0001::executeApiCommand(ApiCommand) {}

void M0001::initSocket()
{
  udpSocket = new QUdpSocket(this);
  udpSocket = new QUdpSocket(this);
  udpSocket->bind(45454, QUdpSocket::ShareAddress);
  // udpSocket->bind(QHostAddress::LocalHost, 8855);
  // udpSocket->bind(QHostAddress("192.168.122.8"), 8855);
  connect(udpSocket, &QUdpSocket::readyRead, this, &M0001::readPendingDatagrams);
}

void M0001::readPendingDatagrams()
{
  static int i = 0;
  while (udpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    qDebug() << "---------------------------------------------------";
    qDebug() << "data form socket " << i++;
    qDebug() << datagram.data().toInt();
    qDebug() << "destinationAdress=" << datagram.destinationAddress() << "destinationPort=" << datagram.destinationPort() << "senderAdress=" << datagram.senderAddress() << " senderPort=" << datagram.senderPort();
    // processTheDatagram(datagram);
  }
}
