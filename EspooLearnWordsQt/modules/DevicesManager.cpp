#include "DevicesManager.h"
#include <QNetworkDatagram>
#include <QtQml>

DevicesManager::DevicesManager(QObject *parent) : QAbstractListModel(parent)
{
  udpSocket = new QUdpSocket(this);
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
    initSocket();
    sendTimerEvent(); // call imidietli function to get imidietli IP
    sendTimer->start(250);
  }
  else {
    uninitSocket();
    sendTimer->stop();
  }
  Q_EMIT seachChanged();
}

bool DevicesManager::searchDevices() const { return searchStatus; }

void DevicesManager::initSocket()
{
  udpSocket->bind(45454, QUdpSocket::ShareAddress);
  connect(udpSocket, &QUdpSocket::readyRead, this, &DevicesManager::readPendingDatagrams);
}

void DevicesManager::uninitSocket()
{
  disconnect(udpSocket, &QUdpSocket::readyRead, this, &DevicesManager::readPendingDatagrams);
  udpSocket->close();
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

  while (udpSocket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = udpSocket->receiveDatagram();
    QString deviceName(datagram.data());

    if (!deviceArleadyAdded(datagram.senderAddress().toString())) {
      append(deviceName, "M0001", datagram.senderAddress().toString(), QString::number(datagram.senderPort()), "57230457");
    }
  }
}

bool DevicesManager::deviceArleadyAdded(const QString &ipAddress)
{
  for (auto &device : m_devices) {
    if (device.ipAddress == ipAddress) {
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

void DevicesManager::clear()
{
  beginRemoveRows(QModelIndex(), 0, m_devices.count() - 1);
  m_devices.clear();
  endRemoveRows();
}

void registerQmlDevicesManagerTypes() { qmlRegisterType<DevicesManager>("Backend", 1, 0, "DevicesManager"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlDevicesManagerTypes)
