#include "ApplicationB0002.h"

// ApplicationB0002::ApplicationB0002() : oneWire(oneWirePin), sensors(&oneWire)
// {
//     sensors.setWaitForConversion(false);
// }

// void ApplicationB0002::begin()
// {
//     Application::begin();
// }

// std::string ApplicationB0002::getBrickType() const
// {
//     return "B0002";
// }

// void ApplicationB0002::handleNewTcpClient(void *arg, AsyncClient *tcpClient)
// {
//     B0002 *newClient = new B0002(tcpClient);
//     Application::addNewBrickClient(newClient);
//     clients.insert(newClient);
// }

// void ApplicationB0002::handleNewTcpSocket(TcpSocket *socket)
// {
//     Serial.printf("\n%lu ApplicationB0002::handleNewTcpSocket() ", dtime());
// }

// void ApplicationB0002::update()
// {
//     Application::update();
//     // static int i = 0;
//     // Serial.println(String("ApplicationB0002::update() ") + String(i++));

//     if (sensors.isConversionComplete())
//     {
//         float temperatureCelsius = sensors.getTempCByIndex(0);
//         // Serial.print("Temperatura: ");
//         // Serial.print(temperatureCelsius);
//         // Serial.println(" °C");

//         for (auto it = clients.begin(); it != clients.end(); ++it)
//         {
//             B0002 *ptr = *it;
//             ptr->cmdSetMeasureTemp(temperatureCelsius);
//         }
//         sensors.requestTemperatures();
//     }
// }
