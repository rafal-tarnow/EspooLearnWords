#include "T0002Controller.hpp"
#include <QtQml>

T0002Controller::T0002Controller(QObject *parent, QString id, QString name): Controller(parent, id, name)
{

}

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

float T0002Controller::frequency()
{
    return m_stateMirror.frequency;
}

void T0002Controller::setFrequency(float freq)
{
    if(m_stateMirror.frequency != freq){
        cmdSetFrequency(freq);
    }
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
        float pressure = ProtocolStd::getFloat(frame);
        float humidity = ProtocolStd::getFloat(frame);
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
    }else if(functionCode == 0x11){ //test value
     //test value
    }else if(functionCode == 0x12){
        qDebug() << "ZZZZZZZZZZZZZZZZZ functionCode == 0x12";
        float frequency = ProtocolStd::getFloat(frame);
        if(m_stateMirror.frequency != frequency){
            m_stateMirror.frequency = frequency;
            emit frequencyChanged();
        }
    }
    return functionCode;
}

void T0002Controller::cmdSetFrequency(float freq)
{
    QByteArray frame;
    ProtocolStd::append(frame, uint8_t(0x11));
    ProtocolStd::append(frame, freq);
    sendStdFrame(frame);
}

static void registerT0002ControllerTypes()
{
    qmlRegisterType<T0002Controller>("Backend", 1, 0, "T0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerT0002ControllerTypes)
