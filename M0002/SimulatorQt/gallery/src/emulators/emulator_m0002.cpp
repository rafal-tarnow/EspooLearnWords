#include "emulator_m0002.hpp"
#include "../global_config.hpp"
#include <QtQml>

EmulatorM0002::EmulatorM0002(QObject *parent) : QObject(parent), mModuleType("B0002")
{
    qInfo() << "Create EmulatorM0002";
    pseudoDNSServer = std::make_unique<PseudoDNSServer>(this);
    connect(pseudoDNSServer.get(),&PseudoDNSServer::hostFound, this, &EmulatorM0002::onHostFound);

    tcpConnection = std::make_unique<TcpConncetion>(this);
    connect(tcpConnection.get(),&TcpConncetion::onTcpConnected, this, &EmulatorM0002::onTcpConnected);
    connect(tcpConnection.get(),&TcpConncetion::onTcpDisconnected,this, &EmulatorM0002::onTcpDisconnected);
    connect(tcpConnection.get(), &TcpConncetion::onTcpError, this, &EmulatorM0002::onTcpError);
    connect(tcpConnection.get(), &TcpConncetion::onTcpFrame, this, &EmulatorM0002::onTcpFrame);

    measureTimer = std::make_unique<QTimer>(this);
    connect(measureTimer.get(), &QTimer::timeout, this, &EmulatorM0002::onMeasureTimer);

    machine.connectToState("idle", [this](bool active) {
        if(active){
            enterIdleState();
        }else{
            exitIdleState();
        }
    });

    machine.connectToState("run", [this](bool active) {
        if(active){
            enterRunState();
        }else{
            exitRunState();
        }
    });

    machine.connectToState("search", [this](bool active) {
        if(active){
            enterSearchState();
        }else{
            exitSearchState();
        }
    });

    machine.connectToState("connecting", [this](bool active) {
        if(active){
            enterConnectingState();
        }else{
            exitConnectingState();
        }
    });

    machine.connectToState("connected", [this](bool active) {
        if(active){
            enterConnectedState();
        }else{
            exitConnectedState();
        }
    });
    machine.start();
}

EmulatorM0002::~EmulatorM0002()
{
    qInfo() << "Delete EmulatorM0002";
}

void EmulatorM0002::enterIdleState()
{

}

void EmulatorM0002::exitIdleState()
{

}

void EmulatorM0002::enterRunState()
{
    pseudoDNSServer->run(mModuleName);

}

void EmulatorM0002::exitRunState()
{
    pseudoDNSServer->stopRunning();
}

void EmulatorM0002::enterSearchState()
{
    pseudoDNSServer->startQueriesForAllHosts();
}

void EmulatorM0002::exitSearchState()
{
    pseudoDNSServer->stopQueries();
}

void EmulatorM0002::enterConnectingState()
{
    tcpConnection->connectToServer(mServerIP, DEFAULT_TCP_PORT);
}

void EmulatorM0002::onTcpConnectingTimeout(){
    machine.submitEvent("connectingTimeoutEvent");
}

void EmulatorM0002::exitConnectingState()
{

}

void EmulatorM0002::enterConnectedState()
{

}

void EmulatorM0002::exitConnectedState()
{
    tcpConnection->disconnectFromServer();
    measureTimer->stop();
}

void EmulatorM0002::on_capi_getNameAndType()
{
    qDebug() << "EmulatorM0002::on_capi_getNameAndType()";

    sapi_setNameAndType(mModuleName,mModuleType);
}

void EmulatorM0002::sapi_setNameAndType(QString &moduleName, QString &moduleType)
{
    qDebug() << "EmulatorM0002::on_capi_getNameAndType()";
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x02));
    ProtocolStd::append(frame, moduleName);
    ProtocolStd::append(frame, moduleType);
    tcpConnection->sendFrame(frame);
}

void EmulatorM0002::sapi_setTemp(float temp)
{
    qDebug() << "EmulatorM0002::sapi_setTemp()";
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x03));
    ProtocolStd::append(frame, float(temp));
    tcpConnection->sendFrame(frame);
}

void EmulatorM0002::on_capi_setMeasureInterval(const QByteArray &frame){
    qDebug() << "EmulatorM0002::on_capi_setMeasureInterval()";
    std::deque<uint8_t> stdFrame(frame.begin(), frame.end());
    uint8_t functionCode = ProtocolStd::getUint8_t(stdFrame);
    bool measure = ProtocolStd::getBool(stdFrame);
    uint32_t interval = ProtocolStd::getUint32_t(stdFrame);

    qDebug() << "functionCode: " << functionCode;
    qDebug() << "measure: " << measure;
    qDebug() << "interval: " << interval;

    if(measure){
        measureTimer->start(interval);
    }else{
        measureTimer->stop();
    }
}

void EmulatorM0002::start(const QString & moduleName, const QString& serverName){
    mServerName = serverName;
    mModuleName = moduleName;
    machine.submitEvent("startEvent");
}

void EmulatorM0002::stop()
{
    machine.submitEvent("stopEvent");
}

void EmulatorM0002::onHostFound(const QString &hostName, const QString &hostIP)
{
    if(hostName == mServerName){
        mServerIP = hostIP;
        machine.submitEvent("hostFoundEvent");
    }
}

void EmulatorM0002::onTcpConnected()
{
    machine.submitEvent("connectedEvent");
}

void EmulatorM0002::onTcpDisconnected()
{
    machine.submitEvent("disconnectedEvent");
}

void EmulatorM0002::onTcpError(const QString & error)
{
    qDebug() << "EmulatorM0002::onTcpError() = " << error;
    machine.submitEvent("tcpErrorEvent");
}

void EmulatorM0002::onTcpFrame(const QByteArray &frame)
{
    qDebug() << "EmulatorM0002::onTcpFrame() mmmmmmmmmmmmmmmm" << frame ;
    if (frame.size() == 1 && frame.at(0) == 0x02) {
        on_capi_getNameAndType();
    }else if(frame.size() >= 1 && frame.at(0) == 0x03){
        on_capi_setMeasureInterval(frame);
    }
}

void EmulatorM0002::onMeasureTimer()
{
    sapi_setTemp(m_temp);
}

static void registerEmulatorM0002Types() {
    qmlRegisterType<EmulatorM0002>("Backend", 1, 0, "EmulatorM0002");
}

Q_COREAPP_STARTUP_FUNCTION(registerEmulatorM0002Types)



float EmulatorM0002::getTemp() const
{
    return m_temp;
}

void EmulatorM0002::setTemp(float newTemp)
{
    if (qFuzzyCompare(m_temp, newTemp))
        return;
    m_temp = newTemp;
    emit tempChanged();
}
