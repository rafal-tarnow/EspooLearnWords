#include "ApplicationB0002.h"

ApplicationB0002::ApplicationB0002() : oneWire(oneWirePin), sensors(&oneWire)
{
    sensors.setWaitForConversion(false);
}

void ApplicationB0002::begin()
{
    Application::begin();
}

BrickClient *ApplicationB0002::createBrickClient(AsyncClient *tcpClient)
{
    Serial.printf("ApplicationB0002::handleBrickCreate()");
    B0002 *newClient = new B0002(tcpClient);
    clients.insert(newClient);
    return newClient;
}

 void ApplicationB0002::deleteBrickClient(BrickClient *brickClient){
  clients.erase(reinterpret_cast<B0002*>(brickClient));
  delete brickClient;
 }

std::string ApplicationB0002::getBrickType() const
{
    return "B0002";
}

void ApplicationB0002::update()
{
    Application::update();
    // static int i = 0;
    // Serial.println(String("ApplicationB0002::update() ") + String(i++));

    if (sensors.isConversionComplete())
    {
        float temperatureCelsius = sensors.getTempCByIndex(0);
        // Serial.print("Temperatura: ");
        // Serial.print(temperatureCelsius);
        // Serial.println(" °C");

        for (auto it = clients.begin(); it != clients.end(); ++it)
        {
            B0002 *ptr = *it;
            ptr->cmdSetMeasureTemp(temperatureCelsius);
        }
        sensors.requestTemperatures();
    }
}
