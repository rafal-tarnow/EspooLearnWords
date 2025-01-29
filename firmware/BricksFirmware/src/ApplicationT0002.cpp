#include "ApplicationT0002.h"
#include "led.h"

void ApplicationT0002::setup()
{
  pinMode(4, OUTPUT);
  timerPWM.onTimeout(this, &ApplicationT0002::handlePWMTimer);
  timerPWM.startMicros(calculatePeriod(5));
}

void ApplicationT0002::handlePWMTimer()
{
#warning 'this code is for testing purposes, sould be removed'
  static bool pinState = LOW;
  pinState = !pinState;
  digitalWrite(4, pinState);
  LedSetState(pinState);
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
    for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
    {
      T0002 *ptr = *it;
      ptr->cmdSetMeasureTempPressHum(temp, pressure, humidity);
    }
  }
}

void ApplicationT0002::handleFrequency(T0002 *client, float freq)
{
  //Serial.printf("%lu %s freq=%f\n", dtime(), __PRETTY_FUNCTION__, freq);
  if (brickState.frequency != freq)
  {
    brickState.frequency = freq;

    uint64_t period_us = calculatePeriod(brickState.frequency);

    if(period_us == 0){
      timerPWM.stop();
    }else{
      timerPWM.startMicros(period_us);
    }
    
    for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
    {
      T0002 *ptr = *it;
      //Serial.printf("%lu %s freq2=%f period=%llu\n", dtime(), __PRETTY_FUNCTION__, freq, period_us);
      ptr->cmdSetFrequency(brickState.frequency);
    }
  }
}

void ApplicationT0002::handleNewBrickClient(T0002 *client)
{
#ifdef __DBG_APPLICATION__J0001__
  Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
  client->onSetFrequency(this, &ApplicationT0002::handleFrequency);
  // client->onSetRelayState(this, &ApplicationM0002::handleRelayState);

    for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
    {
      T0002 *ptr = *it;
      //Serial.printf("%lu %s freq2=%f period=%llu\n", dtime(), __PRETTY_FUNCTION__, freq, period_us);
      ptr->cmdSetFrequency(brickState.frequency);
    }
}

uint64_t ApplicationT0002::calculatePeriod(float freq)
{
    if (freq == 0.0f) {
        return 0;
    }

    const double minFreq = 1e-6;

    if (freq < minFreq) {
        return UINT64_MAX;
    }
    double freq64 = double(freq);
    double period = 1000000.0 / (2.0 * freq64);

    return uint64_t(period);
}
