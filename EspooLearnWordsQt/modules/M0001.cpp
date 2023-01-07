#include "M0001.h"
#include <QNetworkDatagram>

M0001::M0001(QString moduleName)
{
  socket = new QUdpSocket();
  socket->bind(45454, QUdpSocket::ShareAddress);
  connect(socket, &QUdpSocket::readyRead, this, &M0001::readPendingDatagrams);
}

M0001::~M0001() {}

void M0001::executeApiCommand(ApiCommand) {}

void M0001::readPendingDatagrams() {}
