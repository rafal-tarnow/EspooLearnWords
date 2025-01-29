#include "tcp_connection.hpp"
#include <QAbstractSocket>
#include <QOverload>

TcpConncetion::TcpConncetion(QObject *parent)
    : QObject()
{
    qDebug() << "TcpConncetion::TcpConncetion()";
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, this, &TcpConncetion::onSocketConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpConncetion::onSocketDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpConncetion::onTcpReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TcpConncetion::onSocketError);

    protocolStd.setOnFrameCallback(this, &TcpConncetion::onProtocolStdFrame);

    connectTimer = std::make_unique<QTimer>(parent);
    connectTimer->setSingleShot(true);
    connect(connectTimer.get(), &QTimer::timeout, this, [this](){
        // from qt QAbstractSocket::errorOccurred documentation: When this signal is emitted, the socket may not be ready for a reconnect attempt. In that case, attempts to reconnect should be done from the event loop. For example, use a QTimer::singleShot() with 0 as the timeout.
        this->tcpSocket->connectToHost(this->mServerIP, this->mServerPort);
    });
}

TcpConncetion::~TcpConncetion()
{
    qDebug() << "TcpConncetion::~TcpConncetion()";
    delete tcpSocket; //delete socket also close socket
}

void TcpConncetion::connectToServer(QString serverIP, quint16 serverPort)
{
    qDebug() << "TcpConncetion::connectToServer()";
    mServerIP = serverIP;
    mServerPort = serverPort;
    //when we try reconnect in error slot, we must do it in next main loop
    connectTimer->start(0);
}

void TcpConncetion::disconnectFromServer()
{
    qDebug() << "TcpConncetion::disconnectFromServer()";
    tcpSocket->close();
}

void TcpConncetion::abord()
{
    qDebug() << "TcpConncetion::abord()";
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

QString TcpConncetion::getIp() const
{
    return tcpSocket->peerAddress().toString();
}

void TcpConncetion::onSocketConnected()
{
    qDebug() << "TcpConncetion::onSocketConnected()";
    protocolStd.reset();
    emit onTcpConnected();
}

void TcpConncetion::onSocketDisconnected()
{
    qDebug() << "TcpConncetion::onSocketDisconnected()";
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
    qDebug() << "TcpConncetion::onSocketError()";
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

