#include "J0001Controller.hpp"
#include <QtQml>

Q_LOGGING_CATEGORY(J0001ControllerClass, "J0001ControllerClass")


J0001Controller::J0001Controller(QObject *parent, QString id, QString name): Controller(parent, id, name)
{

}

QString J0001Controller::type()
{
    return "J0001";
}

uint8_t J0001Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = Controller::handleProtocolFrame(frame);

    if(functionCode < 0x10){
        return functionCode;
    }

    if (functionCode == 0x11){
        bool run = ProtocolStd::getBool(frame);
        if(m_stateMirror.run != run){
            m_stateMirror.run = run;
            emit runChanged();
        }
    }else if (functionCode == 0x12){
        float dropletVol = ProtocolStd::getFloat(frame);
        if(m_stateMirror.dropletVolumeConfig != dropletVol){
            m_stateMirror.dropletVolumeConfig = dropletVol;
            emit dropletVolumeChanged();
        }
    }else if (functionCode == 0x13){
        uint8_t windowDurationConfig = ProtocolStd::getUint8_t(frame);
        if(m_stateMirror.windowDurationConfig != windowDurationConfig){
            m_stateMirror.windowDurationConfig = windowDurationConfig;
            emit windowDurationChanged();
        }
    }else if (functionCode == 0x14){
        qCDebug(J0001ControllerClass) << "}else if (functionCode == 0x14){";
        uint64_t count = ProtocolStd::getUint64_t(frame);
        float milliliters = ProtocolStd::getFloat(frame);
        qCDebug(J0001ControllerClass) << "milliliters = " << milliliters;
        float flowRate = ProtocolStd::getFloat(frame);
        if(m_stateMirror.counter != count){
            m_stateMirror.counter = count;
            emit countChanged();
        }
        if(m_stateMirror.milliliters != milliliters){
            m_stateMirror.milliliters = milliliters;
            emit millilitersChanged();
        }
        if(m_stateMirror.flowRate != flowRate){
            m_stateMirror.flowRate = flowRate;
            emit flowRateChanged();
        }
    }else if (functionCode == 0x15){
        uint32_t runtime_s = ProtocolStd::getUint32_t(frame);
        if(m_stateMirror.runtime_s != runtime_s){
            m_stateMirror.runtime_s = runtime_s;
            emit runtimeChanged();
        }
    }else if (functionCode == 0x16){
        qCDebug(J0001ControllerClass) << "}else if (functionCode == 0x16){";
        float maxFlowDisplay = ProtocolStd::getFloat(frame);
        if(m_stateMirror.maxFlowDisplay != maxFlowDisplay){
            m_stateMirror.maxFlowDisplay = maxFlowDisplay;
            emit maxFlowDisplayChanged();
        }
    }

    return functionCode;
}

bool J0001Controller::run()
{
    return m_stateMirror.run;
}

float J0001Controller::dropletVolume()
{
    return m_stateMirror.dropletVolumeConfig;
}

uint8_t J0001Controller::windowDuration() const {
    return m_stateMirror.windowDurationConfig;
}

uint64_t J0001Controller::count()
{
    return m_stateMirror.counter;
}

double J0001Controller::milliliters()
{
    return m_stateMirror.milliliters;
}

double J0001Controller::flowRate() {
    return m_stateMirror.flowRate;
}

void J0001Controller::setRun(bool newState)
{
    qCDebug(J0001ControllerClass) << __PRETTY_FUNCTION__ << " newState=" << newState;
    if (m_stateMirror.run != newState) {
        cmdSetRunState(newState);
    }
}

void J0001Controller::setDropletVolume(float dropletVolume)
{
    if (m_stateMirror.dropletVolumeConfig != dropletVolume) {
        cmdSetDropletVolume(dropletVolume);
    }
}

void J0001Controller::setWindowDuration(uint8_t duration) {
    if (m_stateMirror.windowDurationConfig != duration) {
        cmdSetWindowDuration(duration);
    }
}

float J0001Controller::maxFlowDisplay()
{
    return m_stateMirror.maxFlowDisplay;
}

void J0001Controller::setMaxFlowDisplay(float maxFlowDisplay)
{
    cmdSetMaxFlowDisplay(maxFlowDisplay);
}

void J0001Controller::resetMeasurement()
{
    cmdResetMeasurement();
}

uint32_t J0001Controller::runtime()
{
    return m_stateMirror.runtime_s;
}

void J0001Controller::cmdSetRunState(bool run)
{
    qCDebug(J0001ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x10));
    ProtocolStd::append(frame, run);
    sendStdFrame(frame);
}

void J0001Controller::cmdSetDropletVolume(float dropletVolume)
{
    qCDebug(J0001ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x11));
    ProtocolStd::append(frame, dropletVolume);
    sendStdFrame(frame);
}

void J0001Controller::cmdSetWindowDuration(uint8_t windowDuration)
{
    qCDebug(J0001ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x12));
    ProtocolStd::append(frame, windowDuration);
    sendStdFrame(frame);
}

void J0001Controller::cmdResetMeasurement()
{
    qCDebug(J0001ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x13));
    sendStdFrame(frame);
}

void J0001Controller::cmdSetMaxFlowDisplay(float maxFlowDisplay)
{
    qCDebug(J0001ControllerClass) << __PRETTY_FUNCTION__;
#warning "Do optymalizaation, send set value command only when data is different"
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x14));
    ProtocolStd::append(frame, maxFlowDisplay);
    sendStdFrame(frame);
}

static void registerJ0001ControllerTypes()
{
    qmlRegisterType<J0001Controller>("Backend", 1, 0, "J0001Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerJ0001ControllerTypes)


