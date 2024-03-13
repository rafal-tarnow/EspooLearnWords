#include "Controller.hpp"
#include <QtQml>
#include <QTcpSocket>
#include "./src/global_config.hpp"

ConnectionMonitor::ConnectionMonitor(QObject *parent) : QObject(parent){
    pingTimer = std::make_unique<QTimer>(parent);
    connect(pingTimer.get(), &QTimer::timeout, this, &ConnectionMonitor::onPingTimer);
}

ConnectionMonitor::~ConnectionMonitor(){

}

void ConnectionMonitor::onFrammeArrive(){
    qDebug() << "ConnectionMonitor::onFrammeArrive()";
    if(mRun == true){
        mFrameArrived = true;
    }
}

void ConnectionMonitor::onPingTimer(){
    qDebug() << "ConnectionMonitor::onPingTimer()";
    if(mRun == true){
        if(mFrameArrived == false){
            emit  frameTimout();
        }else{
           mFrameArrived = false;
           emit sendPingFrame();
        }
    }
}

void ConnectionMonitor::startConnectionMonitoring(){
    qDebug() << "ConnectionMonitor::startConnectionMonitoring()";
    if(mRun == false){
        mRun = true;
        mFrameArrived = false;
        pingTimer->start(5000);
        emit sendPingFrame();
    }
}

void ConnectionMonitor::stopConnectionMonitoring(){
    qDebug() << "ConnectionMonitor::stopConnectionMonitoring()";
    if(mRun == true){
        mRun = false;
        mFrameArrived = false;
        pingTimer->stop();
    }
}

Controller::Controller(QObject *parent, QString id, QString name)
    : QObject(parent), mConnectionMonitor(parent)
{
    mId = id;
    mName = name;

    tcpConnection = std::make_unique<TcpConncetion>(this);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnected, this, &Controller::handleTcpConnected);
    connect(tcpConnection.get(),&TcpConncetion::onTcpDisconnected,this, &Controller::handleTcpDisconnected);
    connect(tcpConnection.get(), &TcpConncetion::onTcpError, this, &Controller::handleTcpError);
    connect(tcpConnection.get(), &TcpConncetion::onProtocolFrame, this, &Controller::handleProtocolFrame);

    connect(&mConnectionMonitor, &ConnectionMonitor::sendPingFrame, this, &Controller::handleConnectionMonitor_sendPingFrame);
    connect(&mConnectionMonitor, &ConnectionMonitor::frameTimout, this, &Controller::handleConnectionMonitor_frameTimout);
}

void Controller::connectToBrick(const QString &ip)
{
    qDebug() << "Controller::connectToBrick() Connecting to" << ip << "on port " << DEFAULT_TCP_PORT << " this.mId=" << mId;
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
    qDebug() << "Controller::disconnectFromBrick()";
    tcpConnection->disconnectFromServer();
}

void Controller::handleTcpConnected()
{
    qDebug() << "Controller::handleTcpConnected() id = " << mId;
    // Tutaj można dodać kod obsługi połączenia
    mConnected = true;
    mConnecting = false;
    mConnectionMonitor.startConnectionMonitoring();
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
    qDebug() << "Controller::handleTcpDisconnected() this.mId=" << mId;
    mConnected = false;
    mConnecting = false;
    mConnectionMonitor.stopConnectionMonitoring();
    emit brickDisconnected();
    emit brickConnectedChanged();
}

void Controller::handleTcpError(const QString & error)
{
    qDebug() << "Controller::handleTcpError() error=" << error;
    tcpConnection->abord();
    mConnecting = false;
    mConnected = false;
    emit brickTcpErrorOccurred();
}

uint8_t Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    if(functionCode == 0x01){ //we received ping frame
        mConnectionMonitor.onFrammeArrive();
    }else if (functionCode == 0x02){ //get Info
        QString id = ProtocolStd::getQString(frame);
        if(mId != id){
            emit brickError(QDateTime::currentDateTime(),  "Brick Id does not match the Id retrieved from the brick.");
            qDebug() << "Controller::handleProtocolFrame disconnectFromBrick() 1";
            disconnectFromBrick();
            return functionCode;
        }
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
        emit brickInfo(id, deviceType, deviceName, deviceSsid, devicePswd);
        if(mWifiPwd != devicePwd){
            mWifiPwd = devicePwd;
            emit wifiPWDChanged();
        }

    }else if (functionCode == 0x03) {
        QString id = ProtocolStd::getQString(frame);
        if(mId != id){
            emit brickError(QDateTime::currentDateTime(),  "Brick Id does not match the Id retrieved from the brick.");
            qDebug() << "Controller::handleProtocolFrame disconnectFromBrick() 2";
            disconnectFromBrick();
            return functionCode;
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

void Controller::handleConnectionMonitor_sendPingFrame()
{
    sendPingFrame();
}

void Controller::handleConnectionMonitor_frameTimout()
{
    emit birckPingTimeoutErrorOccurred();
    tcpConnection->disconnectFromServer();
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

static void registerControllerTypes()
{
    qmlRegisterUncreatableType<Controller>("Backend", 1, 0, "Controller", "Controller qml type cannot be created");
}

Q_COREAPP_STARTUP_FUNCTION(registerControllerTypes)

