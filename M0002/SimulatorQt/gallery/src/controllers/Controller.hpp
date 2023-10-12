#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include "../ObjectCounter.hpp"
#include "../emulators/tcp_connection.hpp"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastTcpError READ lastTcpError NOTIFY brickTcpErrorOccurred)
    Q_PROPERTY(bool connected READ isBrickConnected NOTIFY brickConnectedChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString identifier READ identifier NOTIFY identifierChanged)
    Q_PROPERTY(QString wifiSSID READ wifiSSID NOTIFY wifiSSIDChanged)
    Q_PROPERTY(QString wifiPWD READ wifiPWD NOTIFY wifiPWDChanged)
public:
    explicit Controller(QObject *parent = nullptr);
    Q_INVOKABLE void connectToBrick(const QString &ip);
    Q_INVOKABLE bool isBrickConnected();
    Q_INVOKABLE void disconnectFromBrick();
    Q_INVOKABLE QString lastTcpError() const;
    Q_INVOKABLE void cmdGetInfo();
    Q_INVOKABLE void cmdGetId();
    Q_INVOKABLE void cmdGetType();
    Q_INVOKABLE void cmdGetName();
    Q_INVOKABLE void cmdGetNetworkConfig();
    Q_INVOKABLE void cmdSaveBrickName(const QString & brickName);
    Q_INVOKABLE void cmdSaveNetworkConfig(const QString & ssid, const QString & pwd);
    Q_INVOKABLE QString name() const;
    Q_INVOKABLE QString identifier() const;
    Q_INVOKABLE QString wifiSSID() const;
    Q_INVOKABLE QString wifiPWD() const;

signals:
    void birckPingTimeoutErrorOccurred();
    void brickTcpErrorOccurred();
    void brickConnected();
    void brickConnectedChanged();
    void brickDisconnected();
    void brickInfo(const QString & id, const QString & type, const QString & name, const QString & ssid, const QString & psswd);
    void brickId(const QString & id);
    void identifierChanged();
    void brickType(const QString & type);
    void brickName(const QString & brickName);
    void brickNetworkSettings(const QString & ssid, const QString & psswd);
    void nameChanged();
    void wifiSSIDChanged();
    void wifiPWDChanged();

protected:
    virtual uint8_t handleProtocolFrame( QByteArray & frame);

private slots:
    void handleTcpConnected();
    void handleTcpDisconnected();
    void handleTcpError(const QString & error);
    void handleTcpConnectingTimeout();

private:
    void sendPingFrame();
    void checkConnectionStatus();

private:
    std::unique_ptr<TcpConncetion> tcpConnection;
    QString mLastTcpError;
    QString mIp;
    bool mConnectionCheck = false;
    bool mConnected = false;
    bool mConnecting = false;
    QString mName;
    QString mWifiName;
    QString mWifiPwd;
    QString mId;
    void pingFrameTimeout();
    DBG_COUNT("Controller");
};
