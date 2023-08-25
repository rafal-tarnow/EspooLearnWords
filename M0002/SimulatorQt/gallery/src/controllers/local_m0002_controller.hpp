#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include "../emulators/tcp_connection.hpp"

class M0002LocalClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastTcpError READ lastTcpError NOTIFY brickTcpErrorOccurred)

public:
    explicit M0002LocalClient(QObject *parent = nullptr);
    Q_INVOKABLE void connectToBrick(const QString &ip);
    Q_INVOKABLE bool isBrickConnected();
    Q_INVOKABLE void disconnectFromBrick();
    Q_INVOKABLE QString lastTcpError() const;
    Q_INVOKABLE void cmdGetTypeAndName();
    Q_INVOKABLE void cmdGetNetworkConfig();
    Q_INVOKABLE void cmdSaveBrickName(const QString & brickName);
    Q_INVOKABLE void cmdSaveNetworkConfig(const QString & ssid, const QString & pwd);

signals:
    void birckPingTimeoutErrorOccurred();
    void brickTcpErrorOccurred();
    void brickConnected();
    void brickDisconnected();
    void brickTypeAndName(const QString & type, const QString & brickName);
    void brickNetworkSettings(const QString & ssid, const QString & psswd);
    void brickMeasureTemp(float temp);

private slots:
    void handleTcpConnected();
    void handleTcpDisconnected();
    void handleTcpError(const QString & error);
    void handleTcpFrame( QByteArray & frame);
    void handleTcpConnectingTimeout();

private:
    void sendPingFrame();
    void checkConnectionStatus();

private:
    std::unique_ptr<TcpConncetion> tcpConnection;
    QString m_lastTcpError;
    QString mIp;
    bool mConnectionCheck = false;
    bool mConnected = false;
    bool mConnecting = false;
    void pingFrameTimeout();
};
