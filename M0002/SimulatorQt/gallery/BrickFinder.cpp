#include "BrickFinder.hpp"
#include <QDebug>
#include <QNetworkDatagram>
#include <QtQml>
#include "./src/global_config.hpp"

Q_LOGGING_CATEGORY(BrickFinderClass, "BrickFinderClass")

static QDebug operator<<(QDebug debug, const QNetworkDatagram& datagram) {
    debug << "Source Address: " << datagram.senderAddress().toString()
          << ", Source Port: " << datagram.senderPort()
          << ", Destination Address: " << datagram.destinationAddress().toString()
          << ", Destination Port: " << datagram.destinationPort()
          << ", Interface Index: " << datagram.interfaceIndex()
          << ", Data: " << datagram.data().toHex();

    return debug;
}

BrickFinder::BrickFinder(QObject *parent) : QAbstractListModel(parent)
{
    qCDebug(BrickFinderClass) << "Create PseudoDNSServer";
    queryTimer = std::make_unique<QTimer>(parent);
    checkDateTimeTimer = std::make_unique<QTimer>(parent);
    udpSocket = std::make_unique<QUdpSocket>(parent);

    connect(queryTimer.get(), &QTimer::timeout, this, &BrickFinder::onRepeatTimer);
    connect(checkDateTimeTimer.get(), &QTimer::timeout, this, &BrickFinder::onCheckLastTimeTimer);
    checkDateTimeTimer->start(1000);
}

BrickFinder::~BrickFinder()
{
    qCDebug(BrickFinderClass) << __FUNCTION__;
    uninitSocket();
}


void BrickFinder::startQueriesForAllHosts()
{
    if(runQueriesForAllHosts == false){
        runQueriesForAllHosts = true;
        beginRemoveRows(QModelIndex(), 0, dnsDiscoverdHosts.size());
        dnsDiscoverdHosts.clear();
        endRemoveRows();
        runQueriesForAllHosts = true;
        initSocket();
        queryTimer->start(DEFAULT_PSEUDO_DNS_QUERY_INTERVAL);
        onRepeatTimer(); //to speed up query, send first frame ad startup
    }
}

bool BrickFinder::isQueriesRunning()
{
    return runQueriesForAllHosts;
}

void BrickFinder::stopQueries()
{
    if(runQueriesForAllHosts == true){
        runQueriesForAllHosts = false;
        beginRemoveRows(QModelIndex(), 0, dnsDiscoverdHosts.size());
        dnsDiscoverdHosts.clear();
        endRemoveRows();
        queryTimer->stop();
        uninitSocket();
    }
}


bool BrickFinder::hasBrickIp(const QString &id){
    for (const Host& host : dnsDiscoverdHosts) {
        if (host.id == id) {
            return true;
        }
    }
    return false;
}

QString BrickFinder::getIpById(const QString &id)
{
    for (const Host& host : dnsDiscoverdHosts) {
        if (host.id == id) {
            return host.ip;
        }
    }
    return "";
}

QString BrickFinder::getIpByName(const QString &hostName){
    for (const Host& host : dnsDiscoverdHosts) {
        if (host.name == hostName) {
            return host.ip;
        }
    }
    return "";
}

int BrickFinder::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return dnsDiscoverdHosts.count();
}

QVariant BrickFinder::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case IdRole: return dnsDiscoverdHosts.at(index.row()).id;
        case TypeRole: return dnsDiscoverdHosts.at(index.row()).type;
        case NameRole: return dnsDiscoverdHosts.at(index.row()).name;
        case IpRole: return dnsDiscoverdHosts.at(index.row()).ip;
        default: return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> BrickFinder::roleNames() const
{
    static const QHash<int, QByteArray> roles {
                                              { IdRole, "IdFromDns" },
                                              { TypeRole, "TypeFromDns" },
                                              { NameRole, "NameFromDns" },
                                              { IpRole, "IpFromDns" },
                                              };
    return roles;
}

void BrickFinder::initSocket()
{
    if(!socketInited){
        connect(udpSocket.get(), &QUdpSocket::readyRead, this, &BrickFinder::readPendingDatagrams);
        socketInited = true;
    }
}

void BrickFinder::sendQueryDatagrams()
{
    qCDebug(BrickFinderClass) << __FUNCTION__;

    getAllBroadcastAdresses(); // we need update all broadcast addreses to void situation when user first start app and then connect to network
    QByteArray data;
    data.append(char(1));
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, DEFAULT_PSEUDO_DNS_PORT);

    // for (auto it = interfaceAdresses.begin(); it != interfaceAdresses.end(); ++it){
    //     int interfaceIndex = it.key();
    //     QNetworkAddressEntry adresses = it.value();

    //     QNetworkDatagram datagram;
    //     //datagram.setInterfaceIndex(interfaceIndex);
    //     datagram.setDestination(adresses.broadcast(), DEFAULT_PSEUDO_DNS_PORT);
    //     datagram.setSender(adresses.ip(), DEFAULT_PSEUDO_DNS_PORT);
    //     datagram.setData(data);
    //     qCDebug(BrickFinderClass) << "DNS 0x01 command send";
    //     udpSocket->writeDatagram(datagram);
    // }
}

void BrickFinder::uninitSocket()
{
    if(socketInited){
        disconnect(udpSocket.get(), &QUdpSocket::readyRead, this, &BrickFinder::readPendingDatagrams);
        udpSocket->close();
        socketInited = false;
    }
}

void BrickFinder::onRepeatTimer()
{
    qCDebug(BrickFinderClass) << __FUNCTION__;
    sendQueryDatagrams();
}

void BrickFinder::onCheckLastTimeTimer()
{
    int currentRow = 0;
    //iterate through all elements
    for (auto it = dnsDiscoverdHosts.begin(); it != dnsDiscoverdHosts.end(); ) {
        qint64 msecondsPassed = it->lastDiscoverTime.msecsTo(QDateTime::currentDateTime());
        if (msecondsPassed > DNS_HOST_TIMEOUT) {
            //if last discovered time is bigger DNS_HOST_TIMEOUT then remove host
            beginRemoveRows(QModelIndex(), currentRow, currentRow);
            it = dnsDiscoverdHosts.erase(it);
            currentRow++;
            endRemoveRows();
        } else {
            //else go next element
            ++it;
            ++currentRow;
        }
    }
}

void BrickFinder::readPendingDatagrams()
{
    qCDebug(BrickFinderClass) << "PseudoDNSServer::readPendingDatagrams()";
    while (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram networkDatagram = udpSocket->receiveDatagram();
        //qCDebug(PseudoDNS) << "Received datagram = " << networkDatagram;
        if(isMyAddress(networkDatagram.senderAddress())){
            continue;
        }
        parseDatagram(networkDatagram);
    }
}

bool BrickFinder::isMyAddress(const QHostAddress & address){    
    for (auto it = interfaceAdresses.constBegin(); it != interfaceAdresses.constEnd(); ++it) {
        if (it.value().ip().isEqual(address)) {
            return true;
        }
    }
    return false;
}

void BrickFinder::parseDatagram(const QNetworkDatagram & receivedDatagram)
{
    if (receivedDatagram.data().isEmpty()) {
        return;
    }

    switch (receivedDatagram.data().at(0)) {
    case 0x02:
        parseResponseWithHost(receivedDatagram);
        break;
    default:
        break;
    }
}

void BrickFinder::parseResponseWithHost(const QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    uint8_t functionCode = ProtocolStd::getUint8_t(data);
    Host host;

    host.id = ProtocolStd::getQString(data);
    host.type = ProtocolStd::getQString(data);
    host.name = ProtocolStd::getQString(data);
    host.ip = datagram.senderAddress().toString();
    host.lastDiscoverTime = QDateTime::currentDateTime();

    qDebug(BrickFinderClass) << __FUNCTION__ << " id=" << host.id << " name=" << host.name;

    if(!dnsDiscoverdHosts.contains(host)){
        addFoundedHost(host);
    }else{
        int currentIndex = 0;
        for (auto it = dnsDiscoverdHosts.begin(); it != dnsDiscoverdHosts.end(); it++) {
            if(it->id == host.id){
                //update last discover time
                it->lastDiscoverTime = QDateTime::currentDateTime();

                if(it->name != host.name){
                    //name changed
                    it->name = host.name;
                    emit dataChanged(index(currentIndex), index(currentIndex), {NameRole});
                    emit hostNameChanged(it->id, it->name);
                }
                if(it->ip != host.ip){
                    //ip changed
                    it->ip = host.ip;
                    emit dataChanged(index(currentIndex), index(currentIndex), {IpRole});
                    emit hostIpChanged(it->id, it->ip);
                }
            }
            currentIndex++;
        }
    }
}

void BrickFinder::addFoundedHost(const Host & host){
    beginInsertRows(QModelIndex(), dnsDiscoverdHosts.size(), dnsDiscoverdHosts.size());
    dnsDiscoverdHosts.push_back(host);
    endInsertRows();
    qDebug() << "DNS FOUND" << " " << host.id << " " << host.type << " " << host.name << " " << host.ip;
    emit hostFound(host.id ,host.type, host.name, host.ip);
}


void BrickFinder::getAllBroadcastAdresses()
{
    QList<QNetworkInterface> listOfInterfaces = QNetworkInterface::allInterfaces();
    for (auto &interface : listOfInterfaces) {
        QList<QNetworkAddressEntry> listOfAddresEntries = interface.addressEntries();
        for (auto &addresEntry : listOfAddresEntries) {
            //qCDebug(PseudoDNS) << "Find address entry: " << addresEntry;
            if (!addresEntry.broadcast().isNull() && !addresEntry.ip().isNull()) {
                //qCDebug(PseudoDNS) << "Accepted address entry: index = " << interface.index() << " " << addresEntry;
                interfaceAdresses.insert(interface.index(), addresEntry);
            }
        }
    }
}

static void registerBrickFinderTypes() {
    qmlRegisterType<BrickFinder>("Backend", 1, 0, "BrickFinder");
}

Q_COREAPP_STARTUP_FUNCTION(registerBrickFinderTypes)
