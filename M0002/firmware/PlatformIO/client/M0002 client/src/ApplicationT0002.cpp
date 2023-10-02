#include "ApplicationT0002.h"

void ApplicationT0002::setup()
{
  
}

void ApplicationT0002::loop()
{
  //Serial.printf("\n%lu --->>> ApplicationT0002::update()", dtime());

 
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  //Serial.printf("\n%lu ApplicationT0002 disconnectRequests = %d", dtime(), disconnectRequests);
  // Sprawdź, czy upłynął wystarczający czas od ostatniego wyświetlenia
  if (currentMillis - previousMillis >= 1)
  {
     //BrickClient::printDbg();
  //ObjectCounter::printArduino();
    // Pobranie ilości wolnej pamięci
  size_t freeHeap = ESP.getFreeHeap();
  static uint64_t index = 0;
  //Serial.printf("Wolna pamięć RAM: %d", freeHeap);

    //    Serial.printf("\n%lu Application::connections = %d", dtime(), connections);
    //Serial.printf("\n%lu Application::disconnections = %d", dtime(), disconnections);
    //Serial.printf("\n%lu Application::clients.size() = %d", dtime(), clients.size());
    //Serial.printf("\n%lu ApplicationT0002 4", dtime());
    previousMillis = currentMillis; // Zaktualizuj poprzednią wartość millis()
                                    // Serial.printf("\n%lu ApplicationT0002 5", dtime());
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
      //Serial.printf("\n%lu clients.size() = %d", dtime(), clients.size());
      //Serial.printf("\n%lu ApplicationT0002 6", dtime());
      T0002 *ptr = *it;
      //Serial.printf("\n%lu BrickClient->space() = %d", dtime(), ptr->space());
      //ptr->cmdSetMeasureTempHumPress(22.5f, 60.0f, 1015.123f);
      //ptr->cmdSetTestValue(index++, "ABCD");
     //Serial.printf("\n%lu ApplicationT0002 8", dtime());
    }
    // Serial.printf("\n%lu ApplicationT0002 9", dtime());
  }
  // Serial.printf("\n%lud ApplicationT0002 10",dtime());

}
