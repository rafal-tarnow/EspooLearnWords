#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001(QString name)
{
  moduleName = name;
  getIpSocket = new QUdpSocket();
  getIpAddresTimer = new QTimer(this);
  commandSocket = new QUdpSocket();

  getDeviceIpAddress();
}

M0001::~M0001()
{
  delete commandSocket;
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

  qDebug() << "M0001: ipAdress.isNull = " << ipAddress.isNull();

  while (getIpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = getIpSocket->receiveDatagram();
    QString recivedDeviceName(datagram.data());
    if (recivedDeviceName == moduleName) {
      ipAddress = datagram.senderAddress();
      ipAddressOk = true;
      qDebug() << "deviceName = " << recivedDeviceName << " IP = " << ipAddress;
    }
  }
  stopGettingIpAddress();
}

void M0001::getIpAddressTimerCallback()
{
  qDebug() << "M0001::getIpAddressTimerCallback() ";

  QByteArray data;
  data.append(char(1));

  qDebug() << "M0001::Timer event = " << getIpSocket->writeDatagram(data, QHostAddress::Broadcast, 45455);
}

void M0001::executeApiCommand(ApiCommand cmd)
{
  qDebug() << "M0001::executeApiCommand = " << cmd;
  if (ipAddressOk) {
    if (cmd == TURN_OFF_LED) {
      QByteArray data;
      data.append(char(2));
      data.append(char(0));
      qDebug() << "M0001::befor writeDatagram ";
      commandSocket->writeDatagram(data, ipAddress, 45455);
      qDebug() << "M0001::after writeDatagram ";
    }
    else if (cmd == TURN_ON_LED) {
      QByteArray data;
      data.append(char(2));
      data.append(char(1));
      commandSocket->writeDatagram(data, ipAddress, 45455);
    }
  }
}

void M0001::readPendingDatagrams() {}
