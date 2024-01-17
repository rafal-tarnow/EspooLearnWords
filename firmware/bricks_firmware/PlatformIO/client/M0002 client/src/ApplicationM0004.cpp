#include "ApplicationM0004.hpp"

ApplicationM0004::ApplicationM0004() : oneWire(oneWirePin), sensors(&oneWire)
{
    sensors.setWaitForConversion(false);
}

void ApplicationM0004::setup()
{
}

void ApplicationM0004::loop()
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
            M0004 *ptr = *it;
            ptr->cmdSetMeasureTemp(temperatureCelsius);
        }
        sensors.requestTemperatures();
    }
}
