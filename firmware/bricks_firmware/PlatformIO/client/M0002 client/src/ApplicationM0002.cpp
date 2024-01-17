#include "ApplicationM0002.h"

ApplicationM0002::ApplicationM0002() : oneWire(oneWirePin), sensors(&oneWire)
{
    sensors.setWaitForConversion(false);
}

void ApplicationM0002::setup()
{
}

void ApplicationM0002::loop()
{

     static float temp_index = 0.0f;
   
    //     // Serial.println(String("ApplicationB0002::update() ") + String(i++));

    if (sensors.isConversionComplete())
    {
        float temperatureCelsius = sensors.getTempCByIndex(0);
         temp_index += 1000.0f;
        Serial.print("Temperatura: ");
        Serial.print(temperatureCelsius);
        Serial.println(" °C");

        for (auto it = clients.begin(); it != clients.end(); ++it)
        {
            M0002 *ptr = *it;
            ptr->cmdSetMeasureTemp(temperatureCelsius);
        }
        sensors.requestTemperatures();
    }
}
