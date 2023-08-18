#include "local_m0002_controller.hpp"
#include <QtQml>
#include <QTcpSocket>
#include "../global_config.hpp"

LocalM0002Controller::LocalM0002Controller(QObject *parent)
    : QObject(parent)
{
    tcpConnection = std::make_unique<TcpConncetion>(this);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnected, this, &LocalM0002Controller::handleTcpConnected);
    connect(tcpConnection.get(),&TcpConncetion::onTcpDisconnected,this, &LocalM0002Controller::handleTcpDisconnected);
    connect(tcpConnection.get(), &TcpConncetion::onTcpError, this, &LocalM0002Controller::handleTcpError);
    connect(tcpConnection.get(), &TcpConncetion::onTcpFrame, this, &LocalM0002Controller::handleTcpFrame);
}

void LocalM0002Controller::connectToBrick(const QString &ip)
{
    qDebug() << "LocalM0002Controller::connectToBrick() Connecting to" << ip << "on port " << DEFAULT_TCP_PORT << "...";
    if(mConnected == false && mConnecting == false){
        mConnecting = true;
        mIp = ip;
        qDebug() << "tcpConnection->connectToServer mConnected = " << mConnected << " mConnecting = " << mConnecting;
        tcpConnection->connectToServer(mIp, DEFAULT_TCP_PORT);
    }
}

bool LocalM0002Controller::isBrickConnected()
{
    return mConnected;
}

void LocalM0002Controller::disconnectFromBrick()
{
    qDebug() << "LocalM0002Controller::disconnectFromBrick()";
    tcpConnection->disconnectFromServer();
}

void LocalM0002Controller::handleTcpConnected()
{
    qDebug() << "LocalM0002Controller::handleTcpConnected()";
    // Tutaj można dodać kod obsługi połączenia
    mConnected = true;
    mConnecting = false;
    checkConnectionStatus();
    emit brickConnected();
}

void LocalM0002Controller::handleTcpDisconnected()
{
    qDebug() << "Disconnected from server";
    // Tutaj można dodać kod obsługi rozłączenia
    mConnected = false;
    emit brickDisconnected();
}

void LocalM0002Controller::handleTcpError(const QString & error)
{
    qDebug() << "Error: " << error;
    tcpConnection->abord();
    mConnecting = false;
    mConnected = false;
    emit brickTcpErrorOccurred();
}

void LocalM0002Controller::handleTcpFrame(QByteArray &frame)
{
    qDebug() << "LocalM0002Controller::handleTcpFrame()" << frame;
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    if(functionCode == 0x01){ //we received ping frame
        mConnectionCheck = true;
    }else if (functionCode == 0x02) {
        QString deviceType = ProtocolStd::getQString(frame);
        QString deviceName = ProtocolStd::getQString(frame);
        qDebug() << "emit brickTypeAndName() deviceType=" << deviceType << " deviceName=" << deviceName;
        emit brickTypeAndName(deviceType, deviceName);
    }else if(functionCode == 0x03){
        QString wifiSSID = ProtocolStd::getQString(frame);
        QString wifiPSWD = ProtocolStd::getQString(frame);
        qDebug() << "emit brickNetworkSettings() wifiSSID=" << wifiSSID << " wifiPSWD=" << wifiPSWD;
        emit brickNetworkSettings(wifiSSID, wifiPSWD);
    }else if(functionCode == 0x10){
        float temp = ProtocolStd::getFloat(frame);
        emit brickMeasureTemp(temp);
    }
}

void LocalM0002Controller::handleTcpConnectingTimeout()
{

}

void LocalM0002Controller::sendPingFrame()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x01));
    tcpConnection->sendFrame(frame);
}

static void registerLocalM0002ControllerTypes()
{
    qmlRegisterType<LocalM0002Controller>("Backend", 1, 0, "LocalM0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerLocalM0002ControllerTypes)

QString LocalM0002Controller::lastTcpError() const
{
    return m_lastTcpError;
}

void LocalM0002Controller::cmdGetTypeAndName()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x02));
    tcpConnection->sendFrame(frame);
}

void LocalM0002Controller::cmdGetNetworkConfig()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x03));
    tcpConnection->sendFrame(frame);
}

void LocalM0002Controller::cmdSaveBrickName(const QString &brickName)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x04));
    ProtocolStd::append(frame,brickName);
    tcpConnection->sendFrame(frame);
}

void LocalM0002Controller::cmdSaveNetworkConfig(const QString &ssid, const QString &pwd)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x05));
    ProtocolStd::append(frame,ssid);
    ProtocolStd::append(frame,pwd);
    tcpConnection->sendFrame(frame);
}

void LocalM0002Controller::checkConnectionStatus()
{
    if(mConnected){
        mConnectionCheck = false;
        sendPingFrame();
        QTimer::singleShot(5000, this, &LocalM0002Controller::pingFrameTimeout);
    }
}

void LocalM0002Controller::pingFrameTimeout()
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
