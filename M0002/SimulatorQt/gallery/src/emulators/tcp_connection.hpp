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
    void connectToServer(QString serverIP, quint16 serverPort);
    void disconnectFromServer();
    void abord();
    void sendFrame(const QByteArray & frame);
    QString getIp() const;

signals:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpError(const QString & error);
    void onProtocolFrame(QByteArray & frame);

private slots:
    void onSocketDisconnected();
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError socketError);

private slots:
    void onTcpReadyRead();

private:
    void onProtocolStdFrame(std::deque<uint8_t>& frame);
    QString mServerIP;
    quint16 mServerPort;
    QTcpSocket * tcpSocket;
    ProtocolStd protocolStd;
    std::unique_ptr<QTimer> connectTimer;
};
