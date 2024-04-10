#include "K0007Controller.hpp"
#include <QtQml>

Q_LOGGING_CATEGORY(K0007ControllerClass, "K0007ControllerClass")

K0007Controller::K0007Controller(QObject *parent, QString id, QString name): Controller(parent, id, name)
{
}

bool K0007Controller::switchState()
{
    return mSwitchState;
}


QString K0007Controller::type()
{
    return "K0007";
}

uint8_t K0007Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = Controller::handleProtocolFrame(frame);
    qCDebug(K0007ControllerClass) << __PRETTY_FUNCTION__;
    if(functionCode < 0x10){
        return functionCode;
    }

    if(functionCode == 0x10){
        bool switchState = ProtocolStd::getBool(frame);
        if(mSwitchState != switchState){
            mStatesChanges++;
            mSwitchState = switchState;
            qCDebug(K0007ControllerClass) << " mSwitchState=" << mSwitchState;
            qCDebug(K0007ControllerClass) << " mStatesChanges=" << mStatesChanges;
            emit switchStateChanged();
        }
    }
    return functionCode;
}

static void registerK0007ControllerTypes()
{
    qmlRegisterType<K0007Controller>("Backend", 1, 0, "K0007Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerK0007ControllerTypes)


