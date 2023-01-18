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
    GET_STATUS,
  } ApiCommand;

  M0001(QString moduleName);
  ~M0001();
  void executeApiCommand(ApiCommand);
  void executeApiCommand(ApiCommand cmd, bool val);
  bool getOnOff();
    bool getApMode();

signals:
  void deviceInited();

private slots:
  void readPendingDatagrams();
  void readDatagramCallback();

private:
  struct {
      bool onOff = false;
      bool apMode = false;
  } deviceStatus;
  bool ipAddressOK = false;
  bool deviceInitOK = false;
  QString moduleName;
  QHostAddress ipAddress;
  QUdpSocket *socket = nullptr;
  QTimer *getIpAddresTimer = nullptr;
  QMap<int, QNetworkAddressEntry> broadcasts; // all broadcast adresses from all network interfaces, int value is interface index index, and QHostAddress is broadcast addres for that interface
  void initIpAddress();
  void initDevice();
  void getIpAddressTimerCallback();
  void initSocket();
  void initGetIpTimer();
  void uninitGetIpTimer();
  void stopGettingIpAddress();
  void updateAllBroadcastAdresses();
  void parseEchoAcqFrame(const QNetworkDatagram &datagram);
  void parseFullStatusAcqFrame(const QByteArray &data);

};
