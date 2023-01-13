#pragma once

#include <QAbstractListModel>
#include <QMap>
#include <QNetworkAddressEntry>
#include <QTimer>
#include <QUdpSocket>
#include <QElapsedTimer>

class DevicesManager : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(bool searchDevices READ searchDevices WRITE setSearchStatus NOTIFY seachChanged)
public:
  enum DeviceRole { DeviceNameRole = Qt::DisplayRole, ModuleType = Qt::UserRole, IpAddressRole, PortRole, SerialNumberRole };
  Q_ENUM(DeviceRole)

  DevicesManager(QObject *parent = nullptr);
  ~DevicesManager();

  int rowCount(const QModelIndex & = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QHash<int, QByteArray> roleNames() const;

  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE void append(const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber);
  Q_INVOKABLE void set(int row, const QString &deviceName, const QString &moduleType, const QString &ipAddress, const QString &port, const QString &serialNumber);
  Q_INVOKABLE void remove(int row);
  Q_INVOKABLE void clear();

  void setSearchStatus(const bool &search);
  bool searchDevices() const;

Q_SIGNALS:
  void seachChanged();

private slots:
  void sendTimerEvent();
  void readPendingDatagrams();

  void timeoutTimerEvent();
private:
  struct Device {
    QString deviceName;
    QString moduleType;
    QString ipAddress;
    QString port;
    QString serialNumber;
    qint64 lastResponseTime;
  };
  QMap<int, QNetworkAddressEntry> broadcasts; // all broadcast adresses from all network interfaces, int value is interface index index, and QHostAddress is broadcast addres for that interface
  QUdpSocket *udpSocket = nullptr;
  QTimer *sendTimer = nullptr;
  QTimer *timoutTimer = nullptr;
  QElapsedTimer elapsedTimer;
  QList<Device> m_devices;
  bool searchStatus = false;
  void initSocket();
  bool deviceArleadyAdded(const QString &deviceName);
  void uninitSocket();
  void updateAllBroadcastAdresses();
  void updateLastResponseTime(const QString &devName, const QString &ipAddress);
};
