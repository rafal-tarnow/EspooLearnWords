#pragma once

#include <QObject>
#include <QElapsedTimer>
#include <memory>

class QTcpSocket;

class TcpTerminal : public QObject
{
    Q_OBJECT
public:
    explicit TcpTerminal(QObject *parent = nullptr);
    ~TcpTerminal();

    Q_INVOKABLE void connectToServer(const QString& ipAddress, const QString& port);
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendPing();

signals:
    void connected();
    void disconnected();
    void pingRecived(qint64 delay);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
private:
    std::unique_ptr<QTcpSocket> m_tcpSocket;
    QElapsedTimer m_elapsedTimer;
};

