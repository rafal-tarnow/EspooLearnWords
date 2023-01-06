#pragma once

#include <QAbstractListModel>
#include <QTimer>
#include <QUdpSocket>

class DevicesManager : public QAbstractListModel {
  Q_OBJECT

public:
  enum DeviceRole { DeviceNameRole = Qt::DisplayRole, IpAddressRole = Qt::UserRole, PortRole, SerialNumberRole };
  Q_ENUM(DeviceRole)

  DevicesManager(QObject *parent = nullptr);
  ~DevicesManager();

  int rowCount(const QModelIndex & = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QHash<int, QByteArray> roleNames() const;

  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE void append(const QString &deviceName, const QString &ipAddress, const QString &port, const QString &serialNumber);
  Q_INVOKABLE void set(int row, const QString &deviceName, const QString &ipAddress, const QString &port, const QString &serialNumber);
  Q_INVOKABLE void remove(int row);

private slots:
  void sendTimerEvent();
  void readPendingDatagrams();

private:
  struct Device {
    QString deviceName;
    QString ipAddress;
    QString port;
    QString serialNumber;
  };
  QUdpSocket *udpSocket = nullptr;
  QTimer *sendTimer = nullptr;
  QList<Device> m_devices;
  QList<Device> test_devices;
  void initSocket();
  bool deviceArleadyAdded(const QString &deviceName);
};
