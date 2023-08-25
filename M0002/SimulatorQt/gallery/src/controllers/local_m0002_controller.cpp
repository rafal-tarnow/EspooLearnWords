#include "local_m0002_controller.hpp"
#include <QtQml>
#include <QTcpSocket>
#include "../global_config.hpp"

M0002LocalClient::M0002LocalClient(QObject *parent)
    : QObject(parent)
{
    tcpConnection = std::make_unique<TcpConncetion>(this);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnected, this, &M0002LocalClient::handleTcpConnected);
    connect(tcpConnection.get(),&TcpConncetion::onTcpDisconnected,this, &M0002LocalClient::handleTcpDisconnected);
    connect(tcpConnection.get(), &TcpConncetion::onTcpError, this, &M0002LocalClient::handleTcpError);
    connect(tcpConnection.get(), &TcpConncetion::onTcpFrame, this, &M0002LocalClient::handleTcpFrame);
}

void M0002LocalClient::connectToBrick(const QString &ip)
{
    qDebug() << "LocalM0002Controller::connectToBrick() Connecting to" << ip << "on port " << DEFAULT_TCP_PORT << "...";
    if(mConnected == false && mConnecting == false){
        mConnecting = true;
        mIp = ip;
        qDebug() << "tcpConnection->connectToServer mConnected = " << mConnected << " mConnecting = " << mConnecting;
        tcpConnection->connectToServer(mIp, DEFAULT_TCP_PORT);
    }
}

bool M0002LocalClient::isBrickConnected()
{
    return mConnected;
}

void M0002LocalClient::disconnectFromBrick()
{
    qDebug() << "LocalM0002Controller::disconnectFromBrick()";
    tcpConnection->disconnectFromServer();
}

void M0002LocalClient::handleTcpConnected()
{
    qDebug() << "LocalM0002Controller::handleTcpConnected()";
    // Tutaj można dodać kod obsługi połączenia
    mConnected = true;
    mConnecting = false;
    checkConnectionStatus();
    emit brickConnected();
}

void M0002LocalClient::handleTcpDisconnected()
{
    qDebug() << "Disconnected from server";
    // Tutaj można dodać kod obsługi rozłączenia
    mConnected = false;
    emit brickDisconnected();
}

void M0002LocalClient::handleTcpError(const QString & error)
{
    qDebug() << "Error: " << error;
    tcpConnection->abord();
    mConnecting = false;
    mConnected = false;
    emit brickTcpErrorOccurred();
}

void M0002LocalClient::handleTcpFrame(QByteArray &frame)
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

void M0002LocalClient::handleTcpConnectingTimeout()
{

}

void M0002LocalClient::sendPingFrame()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x01));
    tcpConnection->sendFrame(frame);
}



QString M0002LocalClient::lastTcpError() const
{
    return m_lastTcpError;
}

void M0002LocalClient::cmdGetTypeAndName()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x02));
    tcpConnection->sendFrame(frame);
}

void M0002LocalClient::cmdGetNetworkConfig()
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x03));
    tcpConnection->sendFrame(frame);
}

void M0002LocalClient::cmdSaveBrickName(const QString &brickName)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x04));
    ProtocolStd::append(frame,brickName);
    tcpConnection->sendFrame(frame);
}

void M0002LocalClient::cmdSaveNetworkConfig(const QString &ssid, const QString &pwd)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x05));
    ProtocolStd::append(frame,ssid);
    ProtocolStd::append(frame,pwd);
    tcpConnection->sendFrame(frame);
}

void M0002LocalClient::checkConnectionStatus()
{
    if(mConnected){
        mConnectionCheck = false;
        sendPingFrame();
        QTimer::singleShot(5000, this, &M0002LocalClient::pingFrameTimeout);
    }
}

void M0002LocalClient::pingFrameTimeout()
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

static void registerM0002LocalClientTypes()
{
    qmlRegisterType<M0002LocalClient>("Backend", 1, 0, "M0002LocalClient");
}

Q_COREAPP_STARTUP_FUNCTION(registerM0002LocalClientTypes)
