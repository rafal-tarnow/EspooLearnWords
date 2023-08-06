#include "pseudo_dns.hpp"
#include <QDebug>
#include <QNetworkDatagram>
#include <QtQml>

Q_LOGGING_CATEGORY(PseudoDNS, "PseudoDNSClass")

static QDebug operator<<(QDebug debug, const QNetworkDatagram& datagram) {
    debug << "Source Address: " << datagram.senderAddress().toString()
          << ", Source Port: " << datagram.senderPort()
          << ", Destination Address: " << datagram.destinationAddress().toString()
          << ", Destination Port: " << datagram.destinationPort()
          << ", Interface Index: " << datagram.interfaceIndex()
          << ", Data: " << datagram.data().toHex();

    return debug;
}

PseudoDNSServer::PseudoDNSServer(QObject *parent) : QObject(parent)
{
    qCDebug(PseudoDNS) << "Create PseudoDNSServer";
    queryTimer = std::make_unique<QTimer>(parent);
    udpSocket = std::make_unique<QUdpSocket>(parent);

    connect(queryTimer.get(), &QTimer::timeout, this, &PseudoDNSServer::onRepeatTimer);
}

PseudoDNSServer::~PseudoDNSServer()
{
    qCDebug(PseudoDNS) << "Delete PseudoDNSServer";
    uninitSocket();
}

void PseudoDNSServer::run(const QString & myName)
{
    isRun = true;
    myHostName = myName;
    initSocket();
}

bool PseudoDNSServer::isRunning()
{
    return isRun;
}

void PseudoDNSServer::stopRunning()
{
    isRun = false;
    if(runQueriesForAllHosts == false){
        uninitSocket();
    }
}

void PseudoDNSServer::startQueriesForAllHosts()
{
    dnsDiscoverdHosts.clear();
    runQueriesForAllHosts = true;
    initSocket();
    queryTimer->start(QUERY_INTERVAL);
    onRepeatTimer(); //to speed up query, send first frame ad startup
}

bool PseudoDNSServer::isQueriesRunning()
{
    return runQueriesForAllHosts;
}

void PseudoDNSServer::stopQueries()
{
    runQueriesForAllHosts = false;
    dnsDiscoverdHosts.clear();
    queryTimer->stop();
    if(isRun == false){
        uninitSocket();
    }
}

void PseudoDNSServer::initSocket()
{
    if(!socketInited){
        udpSocket->bind(PORT, QUdpSocket::ShareAddress);
        connect(udpSocket.get(), &QUdpSocket::readyRead, this, &PseudoDNSServer::readPendingDatagrams);
        socketInited = true;
    }
}

void PseudoDNSServer::sendQueryDatagrams()
{
    getAllBroadcastAdresses(); // we need update all broadcast addreses to void situation when user first start app and then connect to network
    QByteArray data;
    data.append(char(1));


    for (auto it = interfaceAdresses.begin(); it != interfaceAdresses.end(); ++it){
        int interfaceIndex = it.key();
        QNetworkAddressEntry adresses = it.value();

        QNetworkDatagram datagram;
        //datagram.setInterfaceIndex(interfaceIndex);
        datagram.setDestination(adresses.broadcast(), PORT);
        datagram.setSender(adresses.ip(), PORT);
        datagram.setData(data);
        qCDebug(PseudoDNS) << "DNS 0x01 command send";
        udpSocket->writeDatagram(datagram);
    }

}

void PseudoDNSServer::uninitSocket()
{
    if(socketInited){
        disconnect(udpSocket.get(), &QUdpSocket::readyRead, this, &PseudoDNSServer::readPendingDatagrams);
        udpSocket->close();
        socketInited = false;
    }
}

void PseudoDNSServer::onRepeatTimer()
{
    sendQueryDatagrams();
}



void PseudoDNSServer::readPendingDatagrams()
{
    qCDebug(PseudoDNS) << "--------- START READ---------------";
    qCDebug(PseudoDNS) << "PseudoDNSServer::readPendingDatagrams()";
    while (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram networkDatagram = udpSocket->receiveDatagram();
        //qCDebug(PseudoDNS) << "Received datagram = " << networkDatagram;
        if(isMyAddress(networkDatagram.senderAddress())){
            qCDebug(PseudoDNS) << "NOT PARSE DATAGRAM";
            qCDebug(PseudoDNS) << "--------- END READ---------------";
            continue;
        }
        qCDebug(PseudoDNS) << "PARSE DATAGRAM";
        parseDatagram(networkDatagram);
    }
    qCDebug(PseudoDNS) << "--------- END READ---------------";
}

bool PseudoDNSServer::isMyAddress(const QHostAddress & address){
    for (const auto& entry : interfaceAdresses) {
        if (entry.ip().isEqual(address)) {
            return true;
        }
    }
    return false;
}

void PseudoDNSServer::parseDatagram(const QNetworkDatagram & receivedDatagram)
{
    if (receivedDatagram.data().isEmpty()) {
        return;
    }

    switch (receivedDatagram.data().at(0)) {
    case 0x01:
        qCDebug(PseudoDNS) << "DNS 0x01 command received";
        parseQueryForAllHostsDatagram(receivedDatagram);
        break;
    case 0x02:
        qCDebug(PseudoDNS) << "DNS 0x02 command received";
        parseResponseWithHost(receivedDatagram);
        break;
    default:
        break;
    }
}

void PseudoDNSServer::parseResponseWithHost(const QNetworkDatagram &datagram)
{
    QString hostName = QString::fromUtf8(datagram.data().mid(1));
    QString IP = datagram.senderAddress().toString();
    //qCDebug(PseudoDNS) << "FOUND hostName = " << hostName;
    //qCDebug(PseudoDNS) << "FOUND IP = " << IP;
    QPair<QString, QString> hostNameAndIP(hostName, IP);

    if(!dnsDiscoverdHosts.contains(hostNameAndIP)){
        dnsDiscoverdHosts.insert(hostNameAndIP);
        //qCDebug(PseudoDNS) << "EMIT " << hostNameAndIP;
        emit hostFound(hostNameAndIP.first, hostNameAndIP.second);

    }
}

void PseudoDNSServer::parseQueryForAllHostsDatagram(const QNetworkDatagram & receivedDatagram){
    //qCDebug(PseudoDNS) << "PseudoDNSServer::parseQueryForAllHostsDatagram()";
    if(isRun){
        respondQueryForAllHostsDatagram(receivedDatagram);
    }
}



void PseudoDNSServer::respondQueryForAllHostsDatagram(const QNetworkDatagram & receivedDatagram){
    //qCDebug(PseudoDNS) << "PseudoDNSServer::respondQueryForAllHostsDatagram()";
    QByteArray data;

    data.append(0x02);
    data.append(myHostName.toUtf8());

    QNetworkDatagram datagram;
    datagram.setDestination(receivedDatagram.senderAddress(), receivedDatagram.senderPort());
    datagram.setData(data);
    //qCDebug(PseudoDNS) << "SEND datagram = " << datagram;
    udpSocket->writeDatagram(datagram);
}

void PseudoDNSServer::getAllBroadcastAdresses()
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

static void registerPseudoDNSServerlTypes() {
    qmlRegisterType<PseudoDNSServer>("Backend", 1, 0, "PseudoDNSServer");
}

Q_COREAPP_STARTUP_FUNCTION(registerPseudoDNSServerlTypes)
