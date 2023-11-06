#include "M0002Controller.hpp"
#include <QtQml>

float M0002Controller::temperature()
{
    return mTemp;
}

QString M0002Controller::type()
{
    return "M0002";
}

uint8_t M0002Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = Controller::handleProtocolFrame(frame);

    if(functionCode < 0x10){
        return functionCode;
    }

    if(functionCode == 0x10){
        float temp = ProtocolStd::getFloat(frame);
        if(mTemp != temp){
            mTemp = temp;
            emit temperatureChanged();
        }
    }
    return functionCode;
}

static void registerM0002ControllerTypes()
{
    qmlRegisterType<M0002Controller>("Backend", 1, 0, "M0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerM0002ControllerTypes)


