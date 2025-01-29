#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QLoggingCategory>
#include <qqml.h>
#include <memory>
#include <deque>
#include "./src/emulators/ProtocolStd.h"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(ControllerClass)

class ConnectionMonitor: public QObject{
    Q_OBJECT
public:
    ConnectionMonitor(QObject *parent = nullptr);
    ~ConnectionMonitor();

    void onFrammeArrive();
    void startConnectionMonitoring();
    void stopConnectionMonitoring();

signals:
    void frameTimout();
    void sendPingFrame();

private slots:
    void onPingTimer();

private:
    QTimer *pingTimer;
    bool mRun = false;
    bool mFrameArrived = false;
};

class Controller : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString lastTcpError READ lastTcpError NOTIFY tcpErrorOccurred)
    Q_PROPERTY(bool tcpConnected READ isTcpConnected NOTIFY tcpConnectedChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString wifiSSID READ wifiSSID NOTIFY wifiSSIDChanged)
    Q_PROPERTY(QString wifiPWD READ wifiPWD NOTIFY wifiPWDChanged)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QString ip READ ip NOTIFY ipChanged)
public:
    explicit Controller(QObject *parent = nullptr, QString id = "", QString name = "");
    ~Controller();
    Q_INVOKABLE void connectToBrick(const QString &ip);
    Q_INVOKABLE bool isTcpConnected();
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
    Q_INVOKABLE QString ip() const;
    virtual QString type() = 0;

    void tcpAbort();
    QAbstractSocket::SocketState tcpState();

protected:
    void sendStdFrame(const QByteArray &frame);

signals:
    void birckPingTimeoutErrorOccurred();
    void brickError(QDateTime date, QString error);

    void tcpConnected();
    void tcpConnectedChanged();
    void tcpDisconnected();
    void tcpErrorOccurred(const QString & error);

    void brickInfo(const QString & id, const QString & type, const QString & name, const QString & ssid, const QString & psswd);
    void brickType(const QString & type);
    void brickNetworkSettings(const QString & ssid, const QString & psswd);
    void nameChanged();
    void wifiSSIDChanged();
    void wifiPWDChanged();
    void ipChanged();

protected:
    virtual uint8_t handleProtocolFrame( QByteArray & frame);

private slots:
    void handleConnectionMonitor_sendPingFrame();
    void handleConnectionMonitor_frameTimout();

    void handleTcpReadyRead();
    void handleTcpConnected();
    void handleTcpDisconnected();
    void handleTcpError(QAbstractSocket::SocketError socketError);
    void handleTcpStateChanged(QAbstractSocket::SocketState socketState);

public slots:
    void handleTcpConnectingTimeout();

private:
    void sendPingFrame();
    void checkConnectionStatus();
    void onProtocolStdFrame(std::deque<uint8_t>& frame);

private:
    std::unique_ptr<QTimer> connectTimer;
    QTcpSocket * tcpSocket;
    ProtocolStd protocolStd;
    QString mLastTcpError;
    QString mServerIP;
    QString mName;
    QString mWifiName;
    QString mWifiPwd;
    QString mId;
    ConnectionMonitor mConnectionMonitor;
    void pingFrameTimeout();
    DBG_COUNT("Controller");
};
