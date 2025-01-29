#include "ApplicationK0004.hpp"

ApplicationK0004::ApplicationK0004()
{
}

void ApplicationK0004::setup()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    timer.onTimeout(this, &ApplicationK0004::handleRelayTimer);
    timer.start(2500);
}

void ApplicationK0004::loop()
{

    //  static float temp_index = 0.0f;

    // //     // Serial.println(String("ApplicationB0002::update() ") + String(i++));

    // if (sensors.isConversionComplete())
    // {
    //     float temperatureCelsius = sensors.getTempCByIndex(0);
    //      temp_index += 1000.0f;
    //     Serial.print("Temperatura: ");
    //     Serial.print(temperatureCelsius);
    //     Serial.println(" °C");

    //     for (auto it = clients.begin(); it != clients.end(); ++it)
    //     {
    //         M0004 *ptr = *it;
    //         ptr->cmdSetMeasureTemp(temperatureCelsius);
    //     }
    //     sensors.requestTemperatures();
    // }
}

void ApplicationK0004::handleNewBrickClient(M0004 *client)
{
#ifdef __DBG_APPLICATION__K0004__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    client->onSetRelayState(this, &ApplicationK0004::handleRelayState);
}

void ApplicationK0004::handleRelayState(M0004 *client, bool state)
{
#ifdef __DBG_APPLICATION__K0004__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    digitalWrite(RELAY_PIN, state);
}

void ApplicationK0004::handleRelayTimer()
{
#ifdef __DBG_APPLICATION__K0004__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    mRelayState = !mRelayState;
    // digitalWrite(RELAY_PIN, mRelayState);
}
