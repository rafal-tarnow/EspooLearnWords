#include "Controller.hpp"
#include <QtQml>
#include <QTcpSocket>
#include "./src/global_config.hpp"

Q_LOGGING_CATEGORY(ControllerClass, "ControllerClass")

ConnectionMonitor::ConnectionMonitor(QObject *parent) : QObject(parent){
    pingTimer = new QTimer(this);
    connect(pingTimer, &QTimer::timeout, this, &ConnectionMonitor::onPingTimer);
}

ConnectionMonitor::~ConnectionMonitor(){
    qDebug() << "ConnectionMonitor::~ConnectionMonitor()";
    delete pingTimer;
    qDebug() << "END ConnectionMonitor::~ConnectionMonitor()";
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
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__;
    mId = id;
    mName = name;

    protocolStd.setOnFrameCallback(this, &Controller::onProtocolStdFrame);

    connectTimer = std::make_unique<QTimer>(parent);
    connectTimer->setSingleShot(true);
    connect(connectTimer.get(), &QTimer::timeout, this, [this](){
        // from qt QAbstractSocket::errorOccurred documentation: When this signal is emitted, the socket may not be ready for a reconnect attempt. In that case, attempts to reconnect should be done from the event loop. For example, use a QTimer::singleShot() with 0 as the timeout.
        qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " LAMBDA socket state=" << tcpSocket->state();
        //if(this->tcpSocket->state() == QAbstractSocket::UnconnectedState){
            this->tcpSocket->connectToHost(this->mServerIP, DEFAULT_TCP_PORT);
        //}
    });

    tcpSocket = new QTcpSocket(this);
    tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(tcpSocket, &QTcpSocket::connected, this, &Controller::handleTcpConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Controller::handleTcpDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Controller::handleTcpReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &Controller::handleTcpError);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketState>::of(&QTcpSocket::stateChanged), this, &Controller::handleTcpStateChanged);

    connect(&mConnectionMonitor, &ConnectionMonitor::sendPingFrame, this, &Controller::handleConnectionMonitor_sendPingFrame);
    connect(&mConnectionMonitor, &ConnectionMonitor::frameTimout, this, &Controller::handleConnectionMonitor_frameTimout);

}

Controller::~Controller()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__;
    delete tcpSocket; //delete socket also close socket
}

void Controller::connectToBrick(const QString &ip)
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " ip=" << ip;
    mServerIP = ip;
    qCDebug(ControllerClass) << "   socket state=" << tcpSocket->state();
    //when we try reconnect in error slot, we must do it in next main loop
    connectTimer->start(0);
}

bool Controller::isTcpConnected()
{
    return tcpSocket->state() == QAbstractSocket::ConnectedState;
}

void Controller::disconnectFromBrick()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__;
    tcpSocket->disconnectFromHost();
    //tcpSocket->close();
}

void Controller::handleTcpConnected()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " mId=" << mId;
    // Tutaj można dodać kod obsługi połączenia
    protocolStd.reset();
    mConnectionMonitor.startConnectionMonitoring();
    emit tcpConnected();
    emit tcpConnectedChanged();
    if(mServerIP != tcpSocket->peerAddress().toString()){
        mServerIP = tcpSocket->peerAddress().toString();
        emit ipChanged();
    }
    cmdGetInfo();
}

void Controller::handleTcpDisconnected()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " mId=" << mId;
    protocolStd.reset();
    mConnectionMonitor.stopConnectionMonitoring();
    emit tcpDisconnected();
    emit tcpConnectedChanged();
}

void Controller::handleTcpStateChanged(QAbstractSocket::SocketState socketState)
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " socketState=" << socketState;
}

void Controller::handleTcpError(QAbstractSocket::SocketError socketError)
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " error=" << socketError;
    QString error = tcpSocket->errorString();
    tcpSocket->abort();
    emit tcpErrorOccurred(error);
}

uint8_t Controller::handleProtocolFrame(QByteArray &frame)
{
    qDebug() << "Controller::handleProtocolFrame(QByteArray &frame)";
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " functionCode=" << functionCode;
    if(functionCode == 0x01){ //we received ping frame
        mConnectionMonitor.onFrammeArrive();
    }else if (functionCode == 0x02){ //get Info
        QString id = ProtocolStd::getQString(frame);
        if(mId != id){
            qCDebug(ControllerClass) << "Brick Error: Brick Id does not match the Id retrieved from the brick.";
            emit brickError(QDateTime::currentDateTime(),  "Brick Id does not match the Id retrieved from the brick.");
            qDebug() << "Controller::handleProtocolFrame disconnectFromBrick() 1";
            disconnectFromBrick();
            return functionCode;
        }
        QString deviceType = ProtocolStd::getQString(frame);
        QString deviceName = ProtocolStd::getQString(frame);
        QString deviceSsid = ProtocolStd::getQString(frame);
        QString devicePwd = ProtocolStd::getQString(frame);
        qCDebug(ControllerClass) << "Received info frame: deviceType=" << deviceType << " deviceName=" << deviceName << " deviceSsid=" << deviceSsid << " devicePwd=" << devicePwd;
        if(mWifiName != deviceSsid){
            mWifiName = deviceSsid;
            emit wifiSSIDChanged();
        }
        qCDebug(ControllerClass) << " mName=" << mName << " deviceName=" << deviceName;
        if(mName != deviceName){
            mName = deviceName;
            qCDebug(ControllerClass) << "emit nameChanged()";
            emit nameChanged();
        }
        emit brickInfo(id, deviceType, deviceName, deviceSsid, devicePwd);
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
    disconnectFromBrick();
}

void Controller::sendStdFrame(const QByteArray &frame)
{
    QByteArray protocolFrame;
    uint16_t frameSize = static_cast<uint16_t>(frame.size());
    ProtocolStd::append(protocolFrame,frameSize);
    protocolFrame.append(frame);

    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write(protocolFrame);
    }
}

void Controller::handleTcpReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    std::vector<uint8_t> dataStdVector(data.begin(), data.end());
    protocolStd.addData(dataStdVector);
}

void Controller::handleTcpConnectingTimeout()
{

}

void Controller::onProtocolStdFrame(std::deque<uint8_t>& frame)
{
    QByteArray byteArray;
    byteArray.reserve(frame.size());
    for (const auto& byte : frame) {
        byteArray.append(byte);
    }
    handleProtocolFrame(byteArray);
}

void Controller::sendPingFrame()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x01));
    sendStdFrame(frame);
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
    return tcpSocket->peerAddress().toString();
}

void Controller::tcpAbort()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " mId=" << mId;
    tcpSocket->abort();
}

QAbstractSocket::SocketState Controller::tcpState()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__ << " mId=" << mId;
    return tcpSocket->state();
}

void Controller::cmdGetInfo()
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x02));
    sendStdFrame(frame);
}

void Controller::cmdGetId()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x03));
    sendStdFrame(frame);
}

void Controller::cmdGetType()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x04));
    sendStdFrame(frame);
}

void Controller::cmdGetName()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x05));
    sendStdFrame(frame);
}

void Controller::cmdGetNetworkConfig()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x06));
    sendStdFrame(frame);
}

void Controller::cmdSaveBrickName(const QString &brickName)
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x07));
    ProtocolStd::append(frame,brickName);
    sendStdFrame(frame);
}

void Controller::cmdSaveNetworkConfig(const QString &ssid, const QString &pwd)
{
    qCDebug(ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x08));
    ProtocolStd::append(frame,ssid);
    ProtocolStd::append(frame,pwd);
    sendStdFrame(frame);
}

static void registerControllerTypes()
{
    qmlRegisterUncreatableType<Controller>("Backend", 1, 0, "Controller", "Controller qml type cannot be created");
}

Q_COREAPP_STARTUP_FUNCTION(registerControllerTypes)

