#pragma once

#include <memory>
#include <QTcpSocket>
#include <QTimer>
#include <deque>
#include "ProtocolStd.h"

class TcpConncetion : public QObject
{
    Q_OBJECT
public:
    TcpConncetion(QObject *parent = nullptr);
    ~TcpConncetion();
    void connectToServer(QString serverIP, quint16 serverPort, int timeoutMs);
    void disconnectFromServer();
    void sendFrame(const QByteArray & frame);

signals:
    void onTcpConnected();
    void onTcpConnectingTimeout();
    void onTcpDisconnected();
    void onTcpError(const QString & error);
    void onTcpFrame(QByteArray & frame);

private slots:
    void onSocketDisconnected();
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError socketError);

private slots:
    void onTcpReadyRead();
    void onConnectingTimeoutTimer();

private:
    void onProtocolStdFrame(std::deque<uint8_t>& frame);
    QTcpSocket * tcpSocket;
    std::unique_ptr<QTimer> tcpConnectingTimeoutTimer;
    ProtocolStd protocolStd;
};
