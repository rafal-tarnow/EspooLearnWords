#include "local_m0002_controller.hpp"
#include <QtQml>
#include <QTcpSocket>
#include "../global_config.hpp"

LocalM0002Controller::LocalM0002Controller(QObject *parent)
    : QObject(parent)
{
    tcpConnection = std::make_unique<TcpConncetion>(this);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnected, this, &LocalM0002Controller::handleTcpConnected);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnectingTimeout, this, &LocalM0002Controller::handleTcpConnectingTimeout);
    connect(tcpConnection.get(),&TcpConncetion::onTcpDisconnected,this, &LocalM0002Controller::handleTcpDisconnected);
    connect(tcpConnection.get(), &TcpConncetion::onTcpError, this, &LocalM0002Controller::handleTcpError);
    connect(tcpConnection.get(), &TcpConncetion::onTcpFrame, this, &LocalM0002Controller::handleTcpFrame);
}

void LocalM0002Controller::connectToBrick(const QString &ip)
{
    qDebug() << "Connecting to" << ip << "on port " << DEFAULT_TCP_PORT << "...";
    tcpConnection->connectToServer(ip, DEFAULT_TCP_PORT, 60*1000);
}

void LocalM0002Controller::disconnectFromBrick()
{
    tcpConnection->disconnectFromServer();
}

void LocalM0002Controller::handleTcpConnected()
{
    qDebug() << "Connected to server";
    // Tutaj można dodać kod obsługi połączenia
    emit brickConnected();
}

void LocalM0002Controller::handleTcpDisconnected()
{
    qDebug() << "Disconnected from server";
    // Tutaj można dodać kod obsługi rozłączenia
    emit brickDisconnected();
}

void LocalM0002Controller::handleTcpError(const QString & error)
{
    qDebug() << "Error: " << error;
    setLastError(QObject::tr("Error: ") + error);
    // Tutaj można dodać kod obsługi błędu połączenia
}

void LocalM0002Controller::handleTcpFrame(QByteArray &frame)
{
    qDebug() << "LocalM0002Controller::handleTcpFrame()" << frame;
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    if (functionCode == 0x02) {
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

static void registerLocalM0002ControllerTypes()
{
    qmlRegisterType<LocalM0002Controller>("Backend", 1, 0, "LocalM0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerLocalM0002ControllerTypes)

QString LocalM0002Controller::lastError() const
{
    return m_lastError;
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

void LocalM0002Controller::setLastError(const QString &newLastError)
{
    if (m_lastError == newLastError)
        return;
    m_lastError = newLastError;
    emit lastErrorChanged();
}
