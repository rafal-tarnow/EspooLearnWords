#include "tcp_client.hpp"
#include <QtQml>
#include <QTcpSocket>
#include <QElapsedTimer>

TcpClient::TcpClient(QObject *parent)
    : QObject(parent), m_tcpSocket(new QTcpSocket(this))
{
    connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

void TcpClient::connectToServer(const QString& ipAddress, const QString& port)
{
    m_tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    m_tcpSocket->connectToHost(ipAddress, port.toInt());
}

void TcpClient::disconnectFromServer()
{
    m_tcpSocket->disconnectFromHost();
}

void TcpClient::sendPing()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_tcpSocket->write(QByteArray::fromHex("01")); // Wysłanie danej 0x01
        m_elapsedTimer.start(); // Uruchomienie timera
        // Możesz dodać kod obsługi, jeśli chcesz zareagować na wysłanie pinga
    }
}

void TcpClient::onConnected()
{
    emit connected();
}

void TcpClient::onDisconnected()
{
    emit disconnected();
}

void TcpClient::onReadyRead()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState && m_tcpSocket->bytesAvailable() > 0) {
        QByteArray responseData = m_tcpSocket->readAll();
        if (responseData == QByteArray::fromHex("02")) { // Odbiór danej 0x02
            emit pingRecived(m_elapsedTimer.nsecsElapsed()/1000); // Zapisz czas reakcji
            // Możesz dodać kod obsługi, jeśli chcesz reagować na odbiór danych ping
        }
    }
}

void registerQQmlSortFilterProxyModelTypes() {
    qmlRegisterType<TcpClient>("Backend", 1, 0, "TcpClient");
}

Q_COREAPP_STARTUP_FUNCTION(registerQQmlSortFilterProxyModelTypes)


