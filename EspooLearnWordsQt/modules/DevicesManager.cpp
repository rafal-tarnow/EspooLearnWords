#include "DevicesManager.h"
#include <QNetworkDatagram>

DevicesManager::DevicesManager(QObject *parent) : QAbstractListModel(parent)
{
  initSocket();
  sendTimer = new QTimer(this);
  connect(sendTimer, &QTimer::timeout, this, &DevicesManager::sendTimerEvent);
}

DevicesManager::~DevicesManager()
{
  delete sendTimer;
  delete udpSocket;
}

void DevicesManager::setSearchStatus(const bool &search)
{
  searchStatus = search;
  if (searchStatus) {
    sendTimer->start(250);
  }
  else {
    sendTimer->stop();
  }
  Q_EMIT seachChanged();
}

bool DevicesManager::searchDevices() const { return searchStatus; }

void DevicesManager::initSocket()
{
  udpSocket = new QUdpSocket(this);
  udpSocket->bind(45454, QUdpSocket::ShareAddress);
  connect(udpSocket, &QUdpSocket::readyRead, this, &DevicesManager::readPendingDatagrams);
}

void DevicesManager::sendTimerEvent()
{
  qDebug() << "DevicesManager::sendTimerEvent()";
  static uint32_t value = 0;

  union {
    char dataa[4];
    uint32_t val;
  } Frame;
  Frame.val = value;
  value++;
  QByteArray data;
  data.append(Frame.dataa, 4);

  qDebug() << "Timer event = " << udpSocket->writeDatagram(data, QHostAddress::Broadcast, 45455);

  //------------------------------------------------

  //  static int i = 0;
  //  qDebug() << "timer " << i;
  //  if (i < test_devices.size()) {
  //    Device device = test_devices.at(i++);
  //    this->append(device.deviceName, device.ipAddress, device.port, device.serialNumber);
  //  }
}

void DevicesManager::readPendingDatagrams()
{
  qDebug() << "DevicesManager::readPendingDatagrams()";
  static int i = 0;
  while (udpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    qDebug() << "---------------------------------------------------";
    qDebug() << "data form socket " << i++;
    QString deviceName(datagram.data());
    qDebug() << deviceName;
    qDebug() << "destinationAdress=" << datagram.destinationAddress() << "destinationPort=" << datagram.destinationPort() << "senderAdress=" << datagram.senderAddress() << " senderPort=" << datagram.senderPort();

    if (!deviceArleadyAdded(deviceName)) {
      append(deviceName, "M0001", datagram.senderAddress().toString(), QString::number(datagram.senderPort()), "57230457");
    }
  }
}

bool DevicesManager::deviceArleadyAdded(const QString &deviceName)
{
  for (auto &device : m_devices) {
    if (device.deviceName == deviceName) {
      return true;
    }
  }
  return false;
}

int DevicesManager::rowCount(const QModelIndex &) const { return m_devices.count(); }

QVariant DevicesManager::data(const QModelIndex &index, int role) const
{
  if (index.row() < rowCount())
    switch (role) {
    case DeviceNameRole:
      return m_devices.at(index.row()).deviceName;
    case ModuleType:
      return m_devices.at(index.row()).moduleType;
    case IpAddressRole:
      return m_devices.at(index.row()).ipAddress;
    case PortRole:
      return m_devices.at(index.row()).port;
    case SerialNumberRole:
      return m_devices.at(index.row()).serialNumber;
    default:
      return QVariant();
    }
  return QVariant();
}

QHash<int, QByteArray> DevicesManager::roleNames() const
{
  static const QHash<int, QByteArray> roles{{DeviceNameRole, "deviceName"}, {ModuleType, "moduleType"}, {IpAddressRole, "ipAddress"}, {PortRole, "port"}, {SerialNumberRole, "serialNumber"}};
  return roles;
}

QVariantMap DevicesManager::get(int row) const
{
  const Device device = m_devices.value(row);
  return {{"deviceName", device.deviceName}, {"moduleType", device.moduleType}, {"ipAddress", device.ipAddress}, {"port", device.port}, {"serialNumber", device.serialNumber}};
}

void DevicesManager::append(const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
  int row = 0;
  while (row < m_devices.count() && deviceName > m_devices.at(row).deviceName)
    ++row;
  beginInsertRows(QModelIndex(), row, row);
  m_devices.insert(row, {deviceName, moduleType, ipAddress, port, serialNumber});
  endInsertRows();
}

void DevicesManager::set(int row, const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber)
{
  if (row < 0 || row >= m_devices.count())
    return;

  m_devices.replace(row, {deviceName, moduleType, ipAddress, port, serialNumber});
  dataChanged(index(row, 0), index(row, 0), {DeviceNameRole, ModuleType, IpAddressRole, PortRole, SerialNumberRole});
}

void DevicesManager::remove(int row)
{
  if (row < 0 || row >= m_devices.count())
    return;

  beginRemoveRows(QModelIndex(), row, row);
  m_devices.removeAt(row);
  endRemoveRows();
}
