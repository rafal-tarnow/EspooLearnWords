#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001(QString name)
{
  moduleName = name;
  socket = new QUdpSocket();
#warning Diffeerent port than Device , fix it
  socket->bind(4555, QUdpSocket::ShareAddress);
  connect(socket, &QUdpSocket::readyRead, this, &M0001::readPendingDatagrams);

  getDeviceIpAddress();
}

M0001::~M0001() { delete socket; }

void M0001::getDeviceIpAddress()
{
  getIpAddresTimer = new QTimer(this);
  connect(getIpAddresTimer, &QTimer::timeout, this, &M0001::getIpAddressTimerEvent);
}

void M0001::getIpAddressTimerEvent() {}

void M0001::executeApiCommand(ApiCommand cmd) { qDebug() << "M0001::executeApiCommand = " << cmd; }

void M0001::readPendingDatagrams() {}
