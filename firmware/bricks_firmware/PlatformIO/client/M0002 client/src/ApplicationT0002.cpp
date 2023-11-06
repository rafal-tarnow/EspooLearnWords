#include "ApplicationT0002.h"

void ApplicationT0002::setup()
{
}

void ApplicationT0002::loop()
{
  // Serial.printf("\n%lu --->>> ApplicationT0002::update()", dtime());

  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  static float temp = 15.0f;
  static float pressure = 1000.0f;
  static float humidity = 50.0f;

  if (currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    pressure += 1.0f;
    temp += 0.125f;
    humidity += 0.011;
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
      T0002 *ptr = *it;
      ptr->cmdSetMeasureTempPressHum(temp, pressure, humidity);
    }
  }
}
