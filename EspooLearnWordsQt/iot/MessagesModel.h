#pragma once

#include <QAbstractListModel>
#include <QMap>
#include <QTimer>
#include <QUdpSocket>
#include <QElapsedTimer>
#include <qmqtt.h>
#include "./mqtt/QMqttWebsocketClient.h"

class MessagesModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(bool searchDevices READ searchDevices WRITE setSearchStatus NOTIFY seachChanged)
public:
  enum DeviceRole { DeviceNameRole = Qt::DisplayRole, ModuleType = Qt::UserRole, IpAddressRole, PortRole, SerialNumberRole };
  Q_ENUM(DeviceRole)

  MessagesModel(QObject *parent = nullptr);
  ~MessagesModel();

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

private:
  struct Device {
    QString deviceName;
    QString moduleType;
    QString ipAddress;
    QString port;
    QString serialNumber;
    qint64 lastResponseTime;
  };
  QMQTT::Client *client = nullptr; //github lib
  QMqttWebsocketClient *m_client = nullptr; //Qt module
  QElapsedTimer elapsedTimer;
  QList<Device> m_devices;
  bool searchStatus = false;
  bool deviceArleadyAdded(const QString &deviceName);
  void updateLastResponseTime(const QString &devName, const QString &ipAddress);
};
