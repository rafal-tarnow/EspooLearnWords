#include "ApplicationB0002.h"

ApplicationB0002::ApplicationB0002() : oneWire(oneWirePin), sensors(&oneWire)
{
    sensors.setWaitForConversion(false);
}

void ApplicationB0002::begin()
{
    Application::begin(&mBrick);
}

void ApplicationB0002::update()
{
    Application::update();
    //static int i = 0;
    //Serial.println(String("ApplicationB0002::update() ") + String(i++));

    if (sensors.isConversionComplete())
    {
        float temperatureCelsius = sensors.getTempCByIndex(0);
        Serial.print("Temperatura: ");
        Serial.print(temperatureCelsius);
        Serial.println(" °C");

        if (mBrick.isSomeoneConnected()){
            //mBrick.cmdSetMeasureTemp(temperatureCelsius);
        }
        sensors.requestTemperatures();
    }
}
