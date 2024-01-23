#include "Controller.hpp"
#include <QtQml>
#include <QTcpSocket>
#include "../global_config.hpp"
#include "main.hpp"

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    QObject::connect(getQGuiApplication(), &QGuiApplication::applicationStateChanged, this, &Controller::onApplicationStateChanged);

    tcpConnection = std::make_unique<TcpConncetion>(this);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnected, this, &Controller::handleTcpConnected);
    connect(tcpConnection.get(),&TcpConncetion::onTcpDisconnected,this, &Controller::handleTcpDisconnected);
    connect(tcpConnection.get(), &TcpConncetion::onTcpError, this, &Controller::handleTcpError);
    connect(tcpConnection.get(), &TcpConncetion::onProtocolFrame, this, &Controller::handleProtocolFrame); 
}

void Controller::connectToBrick(const QString &ip)
{
    qDebug() << "LocalM0002Controller::connectToBrick() Connecting to" << ip << "on port " << DEFAULT_TCP_PORT << "...";
    if(mConnected == false && mConnecting == false){
        mConnecting = true;
        mIp = ip;
        qDebug() << "tcpConnection->connectToServer mConnected = " << mConnected << " mConnecting = " << mConnecting;
        tcpConnection->connectToServer(mIp, DEFAULT_TCP_PORT);
    }
}

bool Controller::isBrickConnected()
{
    return mConnected;
}

void Controller::disconnectFromBrick()
{
    qDebug() << "LocalM0002Controller::disconnectFromBrick()";
    tcpConnection->disconnectFromServer();
}

void Controller::handleTcpConnected()
{
    qDebug() << "LocalM0002Controller::handleTcpConnected()";
    // Tutaj można dodać kod obsługi połączenia
    mConnected = true;
    mConnecting = false;
    checkConnectionStatus();
    emit brickConnected();
    emit brickConnectedChanged();
    if(tcpConnectionIp != tcpConnection->getIp()){
        tcpConnectionIp = tcpConnection->getIp();
        emit ipChanged();
    }
    cmdGetInfo();
}

void Controller::handleTcpDisconnected()
{
    qDebug() << "Disconnected from server";
    // Tutaj można dodać kod obsługi rozłączenia
    mConnected = false;
    emit brickDisconnected();
    emit brickConnectedChanged();
}

void Controller::handleTcpError(const QString & error)
{
    qDebug() << "Error: " << error;
    tcpConnection->abord();
    mConnecting = false;
    mConnected = false;
    emit brickTcpErrorOccurred();
}

uint8_t Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    if(functionCode == 0x01){ //we received ping frame
        mConnectionCheck = true;
    }else if (functionCode == 0x02){ //get Info
        QString id = ProtocolStd::getQString(frame);
        QString deviceType = ProtocolStd::getQString(frame);
        QString deviceName = ProtocolStd::getQString(frame);
        QString deviceSsid = ProtocolStd::getQString(frame);
        QString devicePwd = ProtocolStd::getQString(frame);
        if(mWifiName != deviceSsid){
            mWifiName = deviceSsid;
            emit wifiSSIDChanged();
        }
        QString devicePswd = ProtocolStd::getQString(frame);
        if(mName != deviceName){
            mName = deviceName;
            emit nameChanged();
        }
        emit brickId(id);
        if(mId != id){
            mId = id;
            emit identifierChanged();
        }
        emit brickInfo(id, deviceType, deviceName, deviceSsid, devicePswd);
        if(mWifiPwd != devicePwd){
            mWifiPwd = devicePwd;
            emit wifiPWDChanged();
        }

    }else if (functionCode == 0x03) {
        QString id = ProtocolStd::getQString(frame);
        emit brickId(id);
        if(mId != id){
            mId = id;
            emit identifierChanged();
        }
    }else if (functionCode == 0x04) {
        QString deviceType = ProtocolStd::getQString(frame);
        emit brickType(deviceType);
    }else if (functionCode == 0x05) { //get Name
        QString deviceName = ProtocolStd::getQString(frame);
        qDebug() << "Recived Name frame" << deviceName;
        if(mName != deviceName){
            mName = deviceName;
            emit nameChanged();
        }
        emit brickName(deviceName);
    }else if(functionCode == 0x06){
        QString wifiSSID = ProtocolStd::getQString(frame);
        QString wifiPSWD = ProtocolStd::getQString(frame);
        emit brickNetworkSettings(wifiSSID, wifiPSWD);
        if(mWifiName != wifiSSID){
            mWifiName = wifiSSID;
            emit wifiSSIDChanged();
        }
        if(mWifiPwd != wifiPSWD){
            mWifiPwd = wifiPSWD;
            emit wifiPWDChanged();
        }
    }
    return functionCode;
}

void Controller::handleTcpConnectingTimeout()
{

}

void Controller::sendPingFrame()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x01));
    tcpConnection->sendFrame(frame);
}

QString Controller::lastTcpError() const
{
    return mLastTcpError;
}

QString Controller::name() const
{
    return mName;
}

QString Controller::identifier() const
{
    return mId;
}

QString Controller::wifiSSID() const
{
    return mWifiName;
}

QString Controller::wifiPWD() const
{
    return mWifiPwd;
}

QString Controller::ip() const
{
    return tcpConnectionIp;
}

void Controller::cmdGetInfo()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x02));
    tcpConnection->sendFrame(frame);
}

void Controller::cmdGetId()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x03));
    tcpConnection->sendFrame(frame);
}

void Controller::cmdGetType()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x04));
    tcpConnection->sendFrame(frame);
}

void Controller::cmdGetName()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x05));
    tcpConnection->sendFrame(frame);
}

void Controller::cmdGetNetworkConfig()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x06));
    tcpConnection->sendFrame(frame);
}

void Controller::cmdSaveBrickName(const QString &brickName)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x07));
    ProtocolStd::append(frame,brickName);
    tcpConnection->sendFrame(frame);
}

void Controller::cmdSaveNetworkConfig(const QString &ssid, const QString &pwd)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x08));
    ProtocolStd::append(frame,ssid);
    ProtocolStd::append(frame,pwd);
    tcpConnection->sendFrame(frame);
}

void Controller::checkConnectionStatus()
{
    if(mConnected){
        mConnectionCheck = false;
        sendPingFrame();
        QTimer::singleShot(5000, this, &Controller::pingFrameTimeout);
    }
}

void Controller::pingFrameTimeout()
{
    if(mConnected){
        if(mConnectionCheck == false){
            emit birckPingTimeoutErrorOccurred();
            tcpConnection->disconnectFromServer();
        }else{
            checkConnectionStatus();
        }
    }
}

static void registerControllerTypes()
{
    qmlRegisterUncreatableType<Controller>("Backend", 1, 0, "Controller", "Controller qml type cannot be created");
}

Q_COREAPP_STARTUP_FUNCTION(registerControllerTypes)

