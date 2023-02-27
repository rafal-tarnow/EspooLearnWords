#pragma once

#include <QAbstractListModel>
#include <QMap>
#include <QTimer>
#include "./mqtt/clientsubscription.h"

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

  Q_INVOKABLE void newConnection(QString topic);
  Q_INVOKABLE void disconnectFromHost();
  Q_INVOKABLE bool isDisconnectedFromHost();


  void setSearchStatus(const bool &search);
  bool searchDevices() const;

Q_SIGNALS:
  void seachChanged();
  void connectedToMqtt();
  void disconnectedFromHost();

private slots:
  void onMqttMessageArrive(const QByteArray & message);

private:
  void parseFrame(QString message);

private:
  struct Device {
    QString valueName;
    QString moduleType;
    QString ipAddress;
    QString port;
    QString serialNumber;
    int row;
  };

  ClientSubscription *clientsub = nullptr; //Qt module, Qt exampl
  QList<Device> mValues;
  bool searchStatus = false;
  bool deviceArleadyAdded(const QString &deviceName);
  int valueExist(const QString &valueName);
};
