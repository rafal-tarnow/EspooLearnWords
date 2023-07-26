#include "pseudo_dns.hpp"
#include <QDebug>
#include <QNetworkDatagram>
#include <QtQml>

static QDebug& operator<<(QDebug& debug, const QNetworkDatagram& datagram) {
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
    qInfo() << "Create PseudoDNSServer";
    queryTimer = std::make_unique<QTimer>(parent);
    udpSocket = std::make_unique<QUdpSocket>(parent);

    connect(queryTimer.get(), &QTimer::timeout, this, &PseudoDNSServer::onRepeatTimer);
}

PseudoDNSServer::~PseudoDNSServer()
{
    qInfo() << "Delete PseudoDNSServer";
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
    if(isRun == false && runQueriesForAllHosts == false &&  runQueriesForHost == false){
        uninitSocket();
    }
}

void PseudoDNSServer::startQueriesForHost(const QString &hostName)
{

}

void PseudoDNSServer::startQueriesForAllHosts()
{
    runQueriesForAllHosts = true;
    initSocket();
    queryTimer->start(250);
}

bool PseudoDNSServer::isQueriesRunning()
{
    return runQueriesForAllHosts || runQueriesForHost;
}

void PseudoDNSServer::stopQueries()
{
    runQueriesForAllHosts = runQueriesForHost = false;
    queryTimer->stop();
    if(runQueriesForAllHosts == false && runQueriesForHost == false && isRun == false){
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
        qDebug() << "Send datagram = " << datagram;
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
    qDebug() << "--------- START READ---------------";
    qDebug() << "PseudoDNSServer::readPendingDatagrams()";
    while (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram networkDatagram = udpSocket->receiveDatagram();
        qDebug() << "Received datagram = " << networkDatagram;
        if(isMyAddress(networkDatagram.senderAddress())){
            qDebug() << "NOT PARSE DATAGRAM";
            qDebug() << "--------- END READ---------------";
            return;
        }
        qDebug() << "PARSE DATAGRAM";
        parseDatagram(networkDatagram);
    }
    qDebug() << "--------- END READ---------------";
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
        qDebug() << "DNS 0x01 command received";
        parseQueryForAllHostsDatagram(receivedDatagram);
        break;
    case 0x02:
        qDebug() << "DNS 0x02 command received";
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
    qDebug() << "FOUND hostName = " << hostName;
    qDebug() << "FOUND IP = " << IP;
    QPair<QString, QString> hostNameAndIP(hostName, IP);

    if(!dnsDiscoverdHosts.contains(hostNameAndIP)){
        dnsDiscoverdHosts.insert(hostNameAndIP);
        qDebug() << "EMIT " << hostNameAndIP;
        emit hostFound(hostNameAndIP.first, hostNameAndIP.second);

    }
}

void PseudoDNSServer::parseQueryForAllHostsDatagram(const QNetworkDatagram & receivedDatagram){
    qDebug() << "PseudoDNSServer::parseQueryForAllHostsDatagram()";
    if(isRun){
        respondQueryForAllHostsDatagram(receivedDatagram);
    }
}



void PseudoDNSServer::respondQueryForAllHostsDatagram(const QNetworkDatagram & receivedDatagram){
    qDebug() << "PseudoDNSServer::respondQueryForAllHostsDatagram()";
    QByteArray data;

    data.append(0x02);
    data.append(myHostName.toUtf8());

    QNetworkDatagram datagram;
    datagram.setDestination(receivedDatagram.senderAddress(), PORT);
    datagram.setData(data);
    qDebug() << "SEND datagram = " << datagram;
    udpSocket->writeDatagram(datagram);
}

void PseudoDNSServer::getAllBroadcastAdresses()
{
    QList<QNetworkInterface> listOfInterfaces = QNetworkInterface::allInterfaces();
    for (auto &interface : listOfInterfaces) {
        QList<QNetworkAddressEntry> listOfAddresEntries = interface.addressEntries();
        for (auto &addresEntry : listOfAddresEntries) {
            qDebug() << "Find address entry: " << addresEntry;
            if (!addresEntry.broadcast().isNull() && !addresEntry.ip().isNull()) {
                qDebug() << "Accepted address entry: index = " << interface.index() << " " << addresEntry;
                interfaceAdresses.insert(interface.index(), addresEntry);
            }
        }
    }
}

static void registerPseudoDNSServerlTypes() {
    qmlRegisterType<PseudoDNSServer>("Backend", 1, 0, "PseudoDNSServer");
}

Q_COREAPP_STARTUP_FUNCTION(registerPseudoDNSServerlTypes)
