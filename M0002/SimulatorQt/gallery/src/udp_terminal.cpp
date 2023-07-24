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
    udpSocket->bind(port.toInt(), QUdpSocket::ShareAddress);
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
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QString dataString = QString::fromUtf8(datagram);
        emit dataReceived(dataString);
    }
}

static void registerUdpTerminalTypes() {
    qmlRegisterType<UdpTerminal>("Backend", 1, 0, "UdpTerminal");
}

Q_COREAPP_STARTUP_FUNCTION(registerUdpTerminalTypes)


