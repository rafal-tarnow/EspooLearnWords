#pragma once

#include <QAbstractListModel>

class DevicesManager : public QAbstractListModel {
  Q_OBJECT

public:
  enum ContactRole { DeviceNameRole = Qt::DisplayRole, AddressRole = Qt::UserRole, CityRole, NumberRole };
  Q_ENUM(ContactRole)

  DevicesManager(QObject *parent = nullptr);

  int rowCount(const QModelIndex & = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QHash<int, QByteArray> roleNames() const;

  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE void append(const QString &fullName, const QString &address, const QString &city, const QString &number);
  Q_INVOKABLE void set(int row, const QString &fullName, const QString &address, const QString &city, const QString &number);
  Q_INVOKABLE void remove(int row);

private:
  struct Device {
    QString deviceName;
    QString address;
    QString city;
    QString number;
  };

  QList<Device> m_devices;
};
