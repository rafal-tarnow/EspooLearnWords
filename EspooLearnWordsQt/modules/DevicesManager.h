#pragma once

#include <QAbstractListModel>

class DevicesManager : public QAbstractListModel {
  Q_OBJECT

public:
  enum ContactRole { DeviceNameRole = Qt::DisplayRole, IpAddressRole = Qt::UserRole, PortRole, SerialNumberRole };
  Q_ENUM(ContactRole)

  DevicesManager(QObject *parent = nullptr);

  int rowCount(const QModelIndex & = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QHash<int, QByteArray> roleNames() const;

  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE void append(const QString &deviceName, const QString &ipAddress, const QString &port, const QString &serialNumber);
  Q_INVOKABLE void set(int row, const QString &deviceName, const QString &ipAddress, const QString &port, const QString &serialNumber);
  Q_INVOKABLE void remove(int row);

private:
  struct Device {
    QString deviceName;
    QString ipAddress;
    QString port;
    QString serialNumber;
  };

  QList<Device> m_devices;
};
