#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001(QString name)
{
  moduleName = name;
  getIpSocket = new QUdpSocket();
  getIpAddresTimer = new QTimer(this);

  getDeviceIpAddress();
}

M0001::~M0001()
{
  delete getIpAddresTimer;
  delete getIpSocket;
}

void M0001::initGetIpSocket()
{
  getIpSocket->bind(45554, QUdpSocket::ShareAddress);
  connect(getIpSocket, &QUdpSocket::readyRead, this, &M0001::readIpDatagramCallback);
}

void M0001::uninitGetIpSocket()
{
  disconnect(getIpSocket, &QUdpSocket::readyRead, this, &M0001::readIpDatagramCallback);
  getIpSocket->close();
}

void M0001::initGetIpTimer()
{
  connect(getIpAddresTimer, &QTimer::timeout, this, &M0001::getIpAddressTimerCallback);
  getIpAddresTimer->start(250);
}

void M0001::uninitGetIpTimer()
{
  getIpAddresTimer->stop();
  disconnect(getIpAddresTimer, &QTimer::timeout, this, &M0001::getIpAddressTimerCallback);
}

void M0001::getDeviceIpAddress()
{
  initGetIpSocket();
  initGetIpTimer();
  getIpAddressTimerCallback(); // call immediatli callback to get fastest Ip
}

void M0001::stopGettingIpAddress()
{
  uninitGetIpTimer();
  // uninitGetIpSocket();
}

void M0001::readIpDatagramCallback()
{
  qDebug() << "M0001::readIpDatagramCallback() ";

  while (getIpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = getIpSocket->receiveDatagram();
    QString recivedDeviceName(datagram.data());
    if (recivedDeviceName == moduleName) {
      ipAddress = datagram.senderAddress();
      qDebug() << "deviceName = " << recivedDeviceName << " IP = " << ipAddress;
    }
  }
  stopGettingIpAddress();
}

void M0001::getIpAddressTimerCallback()
{
  qDebug() << "M0001::getIpAddressTimerCallback() ";

  static uint32_t value = 0;

  union {
    char dataa[4];
    uint32_t val;
  } Frame;
  Frame.val = value;
  value++;
  QByteArray data;
  data.append(Frame.dataa, 4);

  qDebug() << "M0001::Timer event = " << getIpSocket->writeDatagram(data, QHostAddress::Broadcast, 45455);
}

void M0001::executeApiCommand(ApiCommand cmd) { qDebug() << "M0001::executeApiCommand = " << cmd; }

void M0001::readPendingDatagrams() {}
