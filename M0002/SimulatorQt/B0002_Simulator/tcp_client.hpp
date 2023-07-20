#pragma once

#include <QObject>
#include <QElapsedTimer>

class QTcpSocket;

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

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
    QTcpSocket *m_tcpSocket;
    QElapsedTimer m_elapsedTimer;
};

