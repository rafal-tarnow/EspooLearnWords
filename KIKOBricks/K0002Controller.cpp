#include "K0002Controller.hpp"
#include <QtQml>

K0002Controller::K0002Controller(QObject *parent, QString id, QString name): Controller(parent, id, name)
{

}

float K0002Controller::temperature()
{
    return mTemp;
}

QString K0002Controller::type()
{
    return "K0002";
}

uint8_t K0002Controller::handleProtocolFrame(QByteArray &frame)
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

static void registerK0002ControllerTypes()
{
    qmlRegisterType<K0002Controller>("Backend", 1, 0, "K0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerK0002ControllerTypes)


