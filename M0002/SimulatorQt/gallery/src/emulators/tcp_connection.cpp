#include "tcp_connection.hpp"
#include <QAbstractSocket>
#include <QOverload>

TcpConncetion::TcpConncetion(QObject *parent)
    : QObject()
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, this, &TcpConncetion::onSocketConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpConncetion::onSocketDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpConncetion::onTcpReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TcpConncetion::onSocketError);

    protocolStd.setOnFrameCallback(this, &TcpConncetion::onProtocolStdFrame);
}

TcpConncetion::~TcpConncetion()
{
    tcpSocket->close();
    delete tcpSocket;
}

void TcpConncetion::connectToServer(QString serverIP, quint16 serverPort)
{
    tcpSocket->connectToHost(serverIP, serverPort);
}

void TcpConncetion::disconnectFromServer()
{
    tcpSocket->close();
}

void TcpConncetion::abord()
{
    tcpSocket->abort();
}

void TcpConncetion::sendFrame(const QByteArray &frame)
{
    QByteArray protocolFrame;
    uint16_t frameSize = static_cast<uint16_t>(frame.size());
    ProtocolStd::append(protocolFrame,frameSize);
    protocolFrame.append(frame);

    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write(protocolFrame);
    }
}

void TcpConncetion::onSocketConnected()
{
    protocolStd.reset();
    emit onTcpConnected();
}

void TcpConncetion::onSocketDisconnected()
{
    protocolStd.reset();
    emit onTcpDisconnected();
}

void TcpConncetion::onTcpReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    std::vector<uint8_t> dataStdVector(data.begin(), data.end());
    protocolStd.addData(dataStdVector);
}

void TcpConncetion::onSocketError(QAbstractSocket::SocketError socketError)
{
    QString error = tcpSocket->errorString();
    emit onTcpError(error);
}

void TcpConncetion::onProtocolStdFrame(std::deque<uint8_t>& frame)
{
    QByteArray byteArray;
    byteArray.reserve(frame.size());
    for (const auto& byte : frame) {
        byteArray.append(byte);
    }
    emit onProtocolFrame(byteArray);
}
