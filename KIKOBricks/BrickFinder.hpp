#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QNetworkAddressEntry>
#include <QMap>
#include <QUdpSocket>
#include <QLoggingCategory>
#include <QAbstractListModel>
#include <QDateTime>
#include <qqml.h>
#include "./src/emulators/ProtocolStd.h"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(BrickFinderClass)

class BrickFinder : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum DnsRole {
        IdRole = Qt::UserRole,
        TypeRole,
        NameRole,
        IpRole
    };
    Q_ENUM(DnsRole)

    explicit BrickFinder(QObject *parent = nullptr);
    ~BrickFinder();

    Q_INVOKABLE void startQueriesForAllHosts();
    Q_INVOKABLE bool isQueriesRunning();
    Q_INVOKABLE void stopQueries();

    Q_INVOKABLE bool hasBrickIp(const QString &id);
    Q_INVOKABLE QString getIpById(const QString & id);
    Q_INVOKABLE QString getIpByName(const QString& hostName);

    //model methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void hostFound(QString hostId, QString hostType, QString hostName, QString hostIp);
    void hostNameChanged(QString hostId, QString hostName);
    void hostIpChanged(QString hostId, QString hostIp);

private slots:
    void onRepeatTimer();
    void readPendingDatagrams();
    void onCheckLastTimeTimer();

private:
    class Host
    {
    public:
        QString id;
        QString type;
        QString name;
        QString ip;
        QDateTime lastDiscoverTime;

        bool operator==(const Host &other) const
        {
            return id == other.id;
        }
    };

    std::unique_ptr<QTimer> checkDateTimeTimer;
    std::unique_ptr<QTimer> queryTimer;
    std::unique_ptr<QUdpSocket> udpSocket;
    QList<Host> dnsDiscoverdHosts; //QString - hostName, QString - IP
    QMap<int, QNetworkAddressEntry> interfaceAdresses; // int - interface index, QNetworkAddressEntry - adresses at network interface
    bool runQueriesForAllHosts = false;
    bool socketInited = false;
    void getAllBroadcastAdresses();
    void uninitSocket();
    void initSocket();
    void sendQueryDatagrams();
    void parseDatagram(const QNetworkDatagram &datagram);
    void parseResponseWithHost(const QNetworkDatagram & datagram);
    bool isMyAddress(const QHostAddress &address);
    void addFoundedHost(const Host &host);
    DBG_COUNT("BrickFinder");
};
