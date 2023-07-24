#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAddressEntry>
#include <QMap>
#include <QUdpSocket>

class PseudoDNSServer : public QObject
{
    Q_OBJECT
public:
    explicit PseudoDNSServer(QObject *parent = nullptr);
    ~PseudoDNSServer();
    void startQueriesForHost(const QString & hostName);
    void startQueriesForAllHosts();
    void stop();

private slots:
    void onRepeatTimer();
    void readPendingDatagrams();

private:
    std::unique_ptr<QTimer> repeatTimer;
    std::unique_ptr<QUdpSocket> udpSocket;
    QMap<int, QNetworkAddressEntry> broadcasts; // all broadcast adresses from all network interfaces, int value is interface index index, and QHostAddress is broadcast addres for that interface
    bool runQueriesForHost = false;
    bool runQueriesForAllHosts = false;
    const quint16 PORT = 6353;
    void getAllBroadcastAdresses();
    void uninitSocket();
    void initSocket();
    void sendQueryDatagrams();
};
