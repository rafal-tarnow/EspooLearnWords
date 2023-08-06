#include "udp_terminal.hpp"
#include <QtQml>

UdpTerminal::UdpTerminal(QObject *parent) : QObject(parent)
{
    udpSocket = std::make_unique<QUdpSocket>(parent);

    connect(udpSocket.get(), &QUdpSocket::readyRead, this, &UdpTerminal::readPendingDatagrams);
    qDebug() << "Create UdpTerminal";
}

UdpTerminal::~UdpTerminal()
{
    udpSocket->close();
     qDebug() << "Delete UdpTerminal";
}

void UdpTerminal::bind(const QString& port)
{
    qDebug() << "UdpTerminal::bind()";
    udpSocket->close(); //if we send some data before bind, system auto bind do random port, so we must ensure that socket is unbind before bind
    udpSocket->bind(port.toInt(), QUdpSocket::ShareAddress);
}

void UdpTerminal::send(const QString& data, const QString & toIP, const QString & toPort){
    QNetworkDatagram datagram;
    datagram.setDestination(QHostAddress(toIP), toPort.toUInt());
    datagram.setData(data.toUtf8());
    udpSocket->writeDatagram(datagram);
}

void UdpTerminal::sendBroadcast(const QString &data, const QString &toPort)
{
    qDebug() << "UdpTerminal::sendBroadcast()";
    getAllBroadcastAdresses(); // we need update all broadcast addreses to void situation when user first start app and then connect to network

    for (auto it = interfaceAdresses.begin(); it != interfaceAdresses.end(); ++it){
        //int interfaceIndex = it.key();
        QNetworkAddressEntry adresses = it.value();

        QNetworkDatagram datagram;
        //datagram.setInterfaceIndex(interfaceIndex);
        datagram.setDestination(adresses.broadcast(), toPort.toUInt());
        datagram.setData(data.toUtf8());
        udpSocket->writeDatagram(datagram);
    }
}

void UdpTerminal::sendBroadcastHexString(const QString &hexString, const QString &toPort)
{
    getAllBroadcastAdresses(); // we need update all broadcast addreses to void situation when user first start app and then connect to network

    for (auto it = interfaceAdresses.begin(); it != interfaceAdresses.end(); ++it){
        //int interfaceIndex = it.key();
        QNetworkAddressEntry adresses = it.value();

        QNetworkDatagram datagram;
        //datagram.setInterfaceIndex(interfaceIndex);
        datagram.setDestination(adresses.broadcast(), toPort.toUInt());
        datagram.setData(convertHexStringToByteArray(hexString));
        udpSocket->writeDatagram(datagram);
    }
}

void UdpTerminal::sendHexString(const QString &hexString, const QString &toIP, const QString &toPort)
{
    QNetworkDatagram datagram;
    datagram.setDestination(QHostAddress(toIP), toPort.toUInt());
    datagram.setData(convertHexStringToByteArray(hexString));
    udpSocket->writeDatagram(datagram);
}

QByteArray UdpTerminal::convertHexStringToByteArray(const QString& inputString) {
    QByteArray byteArray;
    QStringList byteStrings = inputString.split(" ", Qt::SkipEmptyParts);

    for (const QString& byteString : byteStrings) {
        bool ok;
        quint8 byteValue = byteString.toUInt(&ok, 16);

        if (ok) {
            byteArray.append(byteValue);
        } else {
            qWarning() << "Invalid byte value:" << byteString;
        }
    }

    return byteArray;
}

void UdpTerminal::close()
{
    udpSocket->close();
}

void UdpTerminal::readPendingDatagrams()
{
    qDebug() << "UdpTerminal::readPendingDatagrams()";
    while (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram;
        datagram = udpSocket->receiveDatagram();
        if(isMyAddress(datagram.senderAddress())){ // discard datagrams send to broadcast by my slef
            continue;
        }
        QString dataString = QString::fromUtf8(datagram.data());
        emit dataReceived(dataString, datagram.senderAddress().toString(), QString::number(datagram.senderPort()));
    }
}

void UdpTerminal::getAllBroadcastAdresses()
{
    QList<QNetworkInterface> listOfInterfaces = QNetworkInterface::allInterfaces();
    for (auto &interface : listOfInterfaces) {
        QList<QNetworkAddressEntry> listOfAddresEntries = interface.addressEntries();
        for (auto &addresEntry : listOfAddresEntries) {
            //qDebug() << "Find address entry: " << addresEntry;
            if (!addresEntry.broadcast().isNull() && !addresEntry.ip().isNull()) {
                //qDebug() << "Accepted address entry: index = " << interface.index() << " " << addresEntry;
                interfaceAdresses.insert(interface.index(), addresEntry);
            }
        }
    }
}

bool UdpTerminal::isMyAddress(const QHostAddress & address){
    for (const auto& entry : interfaceAdresses) {
        if (entry.ip().isEqual(address)) {
            return true;
        }
    }
    return false;
}

static void registerUdpTerminalTypes() {
    qmlRegisterType<UdpTerminal>("Backend", 1, 0, "UdpTerminal");
}

Q_COREAPP_STARTUP_FUNCTION(registerUdpTerminalTypes)


