#include "ApplicationJ0001.hpp"

#include <cstdint>
#include "led.h"

volatile static uint64_t m_counter = 0;
static unsigned long lastUpdateTime = 0;

IRAM_ATTR void handleGPIOInterrupt()
{
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    static bool ledState = true;
    ledState = !ledState;
    LedSetState(ledState);
    m_counter++;
}

ApplicationJ0001::ApplicationJ0001()
{
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
}

void ApplicationJ0001::setup()
{
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    pinMode(inputPin, INPUT);
    brickState.m_dropletVolume = kikoLib.readFromConfig("J0001_dropletVolume", float(0.05f));
    brickState.m_windowDuration = kikoLib.readFromConfig("J0001_windowDuration", uint8_t(5));
    brickState.maxFlowDisplay = kikoLib.readFromConfig("J0001_maxFlowDisplay", float(80.0f));
}

void ApplicationJ0001::loop()
{
    // Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdateTime >= 1000)
    {
        lastUpdateTime = currentMillis;

        for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
        {
            J0001Client *ptr = *it;
            ptr->cmdSetRuntime(millis() / 1000);
            if (brickState.m_run)
            {
                float mililiters = m_counter * brickState.m_dropletVolume;
                ptr->cmdSetMeasureValues(m_counter, mililiters, brickState.m_flowRate);
            }
        }
    }
}

void ApplicationJ0001::handleNewBrickClient(J0001Client *client)
{
#ifdef __DBG_APPLICATION__J0001__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    // set callbacks
    client->onSetRunState(this, &ApplicationJ0001::handleRunState);
    client->onSetDropletVolume(this, &ApplicationJ0001::handleDropletVolume);
    client->onSetWindowDuration(this, &ApplicationJ0001::handleWindowDuration);
    client->onResetMeasurement(this, &ApplicationJ0001::handleResetMeasurement);
    client->onSetMaxFlowDisplay(this, &ApplicationJ0001::handleMaxFlowDisplay);

    // update state of new connected client
    client->cmdSetRun(brickState.m_run);
    client->cmdSetDropletVolume(brickState.m_dropletVolume);
    client->cmdSetWindowDuration(brickState.m_windowDuration);
    client->cmdSetMeasureValues(m_counter, milliliters(), brickState.m_flowRate);
    client->cmdSetRuntime(millis() / 1000);
    client->cmdSetMaxFlowDisplay(brickState.maxFlowDisplay);
}

void ApplicationJ0001::handleRunState(J0001Client *client, bool run)
{
    brickState.m_run = run;
    for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
    {
        J0001Client *ptr = *it;
        ptr->cmdSetRun(brickState.m_run);
    }

    if (brickState.m_run)
    {

        attachInterrupt(digitalPinToInterrupt(inputPin), handleGPIOInterrupt, RISING);
    }
    else
    {
        detachInterrupt(digitalPinToInterrupt(inputPin));
    }
}

void ApplicationJ0001::handleDropletVolume(J0001Client *client, float dropletVolume)
{
    // Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    if (brickState.m_dropletVolume != dropletVolume)
    {
        brickState.m_dropletVolume = dropletVolume;
        kikoLib.saveInConfig("J0001_dropletVolume", dropletVolume);
        for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
        {
            J0001Client *ptr = *it;
            ptr->cmdSetDropletVolume(brickState.m_dropletVolume);
        }
    }
}

void ApplicationJ0001::handleWindowDuration(J0001Client *client, uint8_t windowDuration)
{
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    if (brickState.m_windowDuration != windowDuration)
    {
        brickState.m_windowDuration = windowDuration;
        kikoLib.saveInConfig("J0001_windowDuration", windowDuration);
        for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
        {
            J0001Client *ptr = *it;
            ptr->cmdSetWindowDuration(brickState.m_windowDuration);
        }
    }
}

void ApplicationJ0001::handleResetMeasurement(J0001Client *client)
{
    if (brickState.m_run == false)
    {
        m_counter = 0;
        brickState.m_flowRate = 0.0f;

        for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
        {
            J0001Client *ptr = *it;
            ptr->cmdSetMeasureValues(m_counter, milliliters(), brickState.m_flowRate);
        }
    }
}

void ApplicationJ0001::handleMaxFlowDisplay(J0001Client *client, float maxFlowDisplay)
{
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    if (brickState.maxFlowDisplay != maxFlowDisplay)
    {
        brickState.maxFlowDisplay = maxFlowDisplay;
        kikoLib.saveInConfig("J0001_maxFlowDisplay", brickState.maxFlowDisplay);
        for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
        {
            J0001Client *ptr = *it;
            ptr->cmdSetMaxFlowDisplay(brickState.maxFlowDisplay);
        }
    }
}

float ApplicationJ0001::milliliters()
{
    return float(m_counter) * brickState.m_dropletVolume;
}