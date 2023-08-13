#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include "../emulators/tcp_connection.hpp"

class LocalM0002Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError WRITE setLastError NOTIFY lastErrorChanged)

public:
    explicit LocalM0002Controller(QObject *parent = nullptr);
    Q_INVOKABLE void connectToBrick(const QString &ip);
    Q_INVOKABLE void disconnectFromBrick();
    Q_INVOKABLE QString lastError() const;
    Q_INVOKABLE void cmdGetTypeAndName();
    Q_INVOKABLE void cmdGetNetworkConfig();
    Q_INVOKABLE void cmdSaveBrickName(const QString & brickName);
    Q_INVOKABLE void cmdSaveNetworkConfig(const QString & ssid, const QString & pwd);

signals:
    void lastErrorChanged();
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
    void setLastError(const QString &newLastError);

private:
    std::unique_ptr<TcpConncetion> tcpConnection;
    QString m_lastError;
};
