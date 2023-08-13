#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAddressEntry>
#include <QMap>
#include <QUdpSocket>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(PseudoDNS)

class PseudoDNSServer : public QObject
{
    Q_OBJECT
public:
    explicit PseudoDNSServer(QObject *parent = nullptr);
    ~PseudoDNSServer();

    Q_INVOKABLE void run(const QString & myHostName);
    Q_INVOKABLE bool isRunning();
    Q_INVOKABLE void stopRunning();

    Q_INVOKABLE void startQueriesForAllHosts();
    Q_INVOKABLE bool isQueriesRunning();
    Q_INVOKABLE void stopQueries();

signals:
    void hostFound(QString hostName, QString hostIp);

private slots:
    void onRepeatTimer();
    void readPendingDatagrams();

private:
    QString myHostName;
    std::unique_ptr<QTimer> queryTimer;
    std::unique_ptr<QUdpSocket> udpSocket;
    QSet<QPair<QString, QString>> dnsDiscoverdHosts; //QString - hostName, QString - IP
    QMap<int, QNetworkAddressEntry> interfaceAdresses; // int - interface index, QNetworkAddressEntry - adresses at network interface
    bool runQueriesForAllHosts = false;
    bool isRun = false;
    bool socketInited = false;
    void getAllBroadcastAdresses();
    void uninitSocket();
    void initSocket();
    void sendQueryDatagrams();
    void parseDatagram(const QNetworkDatagram &datagram);
    void parseQueryForAllHostsDatagram(const QNetworkDatagram & datagram);
    void parseResponseWithHost(const QNetworkDatagram & datagram);
    void respondQueryForAllHostsDatagram(const QNetworkDatagram &datagram);
    bool isMyAddress(const QHostAddress &address);
};
