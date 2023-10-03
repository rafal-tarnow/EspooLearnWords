#include "ApplicationB0002.h"

ApplicationB0002::ApplicationB0002() : oneWire(oneWirePin), sensors(&oneWire)
{
    sensors.setWaitForConversion(false);
}

void ApplicationB0002::setup()
{
}

void ApplicationB0002::loop()
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
            B0002 *ptr = *it;
            ptr->cmdSetMeasureTemp(temperatureCelsius);
        }
        sensors.requestTemperatures();
    }
}
