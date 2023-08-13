#include "tcp_terminal.hpp"
#include <QtQml>
#include <QTcpSocket>
#include <QElapsedTimer>

TcpTerminal::TcpTerminal(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Create TcpTerminal";
    m_tcpSocket = std::make_unique<QTcpSocket>(this);
    connect(m_tcpSocket.get(), &QTcpSocket::connected, this, &TcpTerminal::onConnected);
    connect(m_tcpSocket.get(), &QTcpSocket::disconnected, this, &TcpTerminal::onDisconnected);
    connect(m_tcpSocket.get(), &QTcpSocket::readyRead, this, &TcpTerminal::onReadyRead);
    connect(m_tcpSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &TcpTerminal::onSocketError);
}

TcpTerminal::~TcpTerminal(){
    m_tcpSocket->close();
    qDebug() << "Delete TcpTerminal";
}

void TcpTerminal::connectToServer(const QString& ipAddress, const QString& port)
{
    m_tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    m_tcpSocket->connectToHost(ipAddress, port.toInt());
}

void TcpTerminal::disconnectFromServer()
{
    m_tcpSocket->disconnectFromHost();
}

void TcpTerminal::onSocketError(QAbstractSocket::SocketError socketError)
{
    emit tcpError(m_tcpSocket->errorString());
}

void TcpTerminal::sendPing()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_tcpSocket->write(QByteArray::fromHex("01")); // Wysłanie danej 0x01
        m_elapsedTimer.start(); // Uruchomienie timera
        // Możesz dodać kod obsługi, jeśli chcesz zareagować na wysłanie pinga
    }
}

void TcpTerminal::onConnected()
{
    emit connected();
}

void TcpTerminal::onDisconnected()
{
    emit disconnected();
}

void TcpTerminal::onReadyRead()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState && m_tcpSocket->bytesAvailable() > 0) {
        QByteArray responseData = m_tcpSocket->readAll();
        emit dataReceived(QString::fromUtf8(responseData));
        if (responseData == QByteArray::fromHex("02")) { // Odbiór danej 0x02
            emit pingRecived(m_elapsedTimer.nsecsElapsed()/1000); // Zapisz czas reakcji
            // Możesz dodać kod obsługi, jeśli chcesz reagować na odbiór danych ping
        }
    }
}

static void registerTcpTerminalTypes() {
    qmlRegisterType<TcpTerminal>("Backend", 1, 0, "TcpTerminal");
}

Q_COREAPP_STARTUP_FUNCTION(registerTcpTerminalTypes)


