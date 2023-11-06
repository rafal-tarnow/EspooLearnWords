#include "T0002Controller.hpp"
#include <QtQml>

float T0002Controller::temperature()
{
    return mTemp;
}

float T0002Controller::humidity()
{
    return mHumidity;
}

float T0002Controller::pressure()
{
    return mPressure;
}

QString T0002Controller::temperatureStr()
{
    return mTempStr;
}

QString T0002Controller::humidityStr()
{
    return mHumidityStr;
}

QString T0002Controller::pressureStr()
{
    return mPressureStr;
}

QString T0002Controller::type()
{
    return "T0002";
}

uint8_t T0002Controller::handleProtocolFrame(QByteArray &frame)
{
    uint8_t functionCode = Controller::handleProtocolFrame(frame);

    if(functionCode < 0x10){
        return functionCode;
    }
    if(functionCode == 0x10){
        float temp = ProtocolStd::getFloat(frame);
        float humidity = ProtocolStd::getFloat(frame);
        float pressure = ProtocolStd::getFloat(frame);
        if(mTemp != temp){
            mTemp = temp;
            emit temperatureChanged();
        }
        if(mPressure != pressure){
            mPressure = pressure;
            emit pressureChanged();
        }
        if(mHumidity != humidity){
            mHumidity = humidity;
            emit humidityChanged();
        }
    }
    return functionCode;
}

static void registerT0002ControllerTypes()
{
    qmlRegisterType<T0002Controller>("Backend", 1, 0, "T0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerT0002ControllerTypes)
