#include "pseudo_dns.hpp"
#include <QDebug>
#include <QNetworkDatagram>

PseudoDNSServer::PseudoDNSServer(QObject *parent) : QObject(parent)
{
    qInfo() << "Create PseudoDNSServer";
    repeatTimer = std::make_unique<QTimer>(parent);
    udpSocket = std::make_unique<QUdpSocket>(parent);

    connect(repeatTimer.get(), &QTimer::timeout, this, &PseudoDNSServer::onRepeatTimer);
}

PseudoDNSServer::~PseudoDNSServer()
{
    qInfo() << "Delete PseudoDNSServer";
    uninitSocket();
}

void PseudoDNSServer::initSocket()
{
    udpSocket->bind(PORT, QUdpSocket::ShareAddress);
    connect(udpSocket.get(), &QUdpSocket::readyRead, this, &PseudoDNSServer::readPendingDatagrams);
}

void PseudoDNSServer::sendQueryDatagrams()
{
    getAllBroadcastAdresses(); // we need update all broadcast addreses to void situation when user first start app and then connect to network

    QByteArray data;
    data.append(char(1));

    QMapIterator<int, QNetworkAddressEntry> iter(broadcasts);

    while (iter.hasNext()) {
        iter.next();

        int interfaceIndex = iter.key();
        QNetworkAddressEntry adresses = iter.value();

        QNetworkDatagram datagram;
        datagram.setInterfaceIndex(interfaceIndex);
        datagram.setDestination(adresses.broadcast(), PORT);
        datagram.setSender(adresses.ip(), PORT);
        datagram.setData(QString("Test Data").toUtf8());
        //datagram.setData(data);
        qDebug() << "send broadcast = " << adresses.broadcast();
        qDebug() << "send = " << udpSocket->writeDatagram(datagram);
    }
}

void PseudoDNSServer::uninitSocket()
{
    disconnect(udpSocket.get(), &QUdpSocket::readyRead, this, &PseudoDNSServer::readPendingDatagrams);
    udpSocket->close();
}

void PseudoDNSServer::startQueriesForHost(const QString &hostName)
{
    runQueriesForHost = true;
    initSocket();
    repeatTimer->start(100);
}

void PseudoDNSServer::startQueriesForAllHosts()
{
    runQueriesForAllHosts = true;
    initSocket();
}

void PseudoDNSServer::stop()
{
    runQueriesForAllHosts = runQueriesForHost = false;
    repeatTimer->stop();
    uninitSocket();
}

void PseudoDNSServer::onRepeatTimer()
{
    sendQueryDatagrams();
}

void PseudoDNSServer::readPendingDatagrams()
{

}

void PseudoDNSServer::getAllBroadcastAdresses()
{
    QList<QNetworkInterface> listOfInterfaces = QNetworkInterface::allInterfaces();
    for (auto &interface : listOfInterfaces) {
        QList<QNetworkAddressEntry> listOfAddresEntries = interface.addressEntries();
        for (auto &addresEntry : listOfAddresEntries) {
            if (!addresEntry.broadcast().isNull()) {
                broadcasts.insert(interface.index(), addresEntry);
            }
        }
    }
}
