#include "K0004Controller.hpp"
#include <QtQml>

Q_LOGGING_CATEGORY(K0004ControllerClass, "K0004ControllerClass")

K0004Controller::K0004Controller(QObject *parent, QString id, QString name): Controller(parent, id, name)
{
}

bool K0004Controller::relayState()
{
    return mRelayState;
}

void K0004Controller::setRelayState(bool newState)
{
    qCDebug(K0004ControllerClass) << __PRETTY_FUNCTION__ << " newState=" << newState;
    if (mRelayState != newState) {
        mRelayState = newState;
        cmdSetRelayState();
        emit relayStateChanged();
    }
}

QString K0004Controller::type()
{
    return "K0004";
}

void K0004Controller::cmdSetRelayState()
{
    qCDebug(K0004ControllerClass) << __PRETTY_FUNCTION__;
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x10));
    ProtocolStd::append(frame, mRelayState);
    sendStdFrame(frame);
}

uint8_t K0004Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = Controller::handleProtocolFrame(frame);

    if(functionCode < 0x10){
        return functionCode;
    }
    return functionCode;
}

static void registerK0004ControllerTypes()
{
    qmlRegisterType<K0004Controller>("Backend", 1, 0, "K0004Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerK0004ControllerTypes)


