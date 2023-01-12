#pragma once
#include <QNetworkAddressEntry>
#include <QTimer>
#include <QUdpSocket>

class M0001 : public QObject {
  Q_OBJECT
public:
  typedef enum {
    TURN_OFF_LED,
    TURN_ON_LED,
    SET_NETWORK_CONFIG,
  } ApiCommand;

  M0001(QString moduleName);
  ~M0001();
  void executeApiCommand(ApiCommand);
  void executeApiCommand(ApiCommand cmd, bool val);

signals:

private slots:
  void readPendingDatagrams();
  void readIpDatagramCallback();

private:
  bool ipAddressOk = false;
  QString moduleName;
  QHostAddress ipAddress;
  QUdpSocket *getIpSocket = nullptr;
  QUdpSocket *commandSocket = nullptr;
  QTimer *getIpAddresTimer = nullptr;
  QMap<int, QNetworkAddressEntry> broadcasts; // all broadcast adresses from all network interfaces, int value is interface index index, and QHostAddress is broadcast addres for that interface
  void getDeviceIpAddress();
  void getIpAddressTimerCallback();
  void initGetIpSocket();
  void uninitGetIpSocket();
  void initGetIpTimer();
  void uninitGetIpTimer();
  void stopGettingIpAddress();
  void updateAllBroadcastAdresses();
};
