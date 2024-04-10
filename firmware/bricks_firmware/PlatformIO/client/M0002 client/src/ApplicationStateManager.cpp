#include "ApplicationStateManager.hpp"
#include "Application.h" // Include the full definition of the Application class
#include "T0002.hpp"
#include "K0002Client.hpp"
#include "M0004.hpp"
#include "K0007.hpp"

template class ApplicationStateManager<T0002>;
template class ApplicationStateManager<K0002Client>;
template class ApplicationStateManager<M0004>;
template class ApplicationStateManager<K0007>;

template <typename T>
ApplicationStateManager<T>::ApplicationStateManager(Application<T> *application) : application(application)
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    switchToConfigTimeoutTimer.setSingleShot(true);
    switchToConfigTimeoutTimer.onTimeout(this, &ApplicationStateManager<T>::handleTransitionToConfigTimeout);

    switchToRunTimeoutTimer.setSingleShot(true);
    switchToRunTimeoutTimer.onTimeout(this, &ApplicationStateManager<T>::handleTransitionToRunTimeout);
}

template <typename T>
void ApplicationStateManager<T>::handleButtonLongPress()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (mAppState != CONFIG_STATE && mAppState != TRANSITION_FROM_RUN_TO_CONFIG_STATE)
    {
        changeState(TRANSITION_FROM_RUN_TO_CONFIG_STATE);
    }
}

template <typename T>
void ApplicationStateManager<T>::handleButtonClick()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (mAppState == CONFIG_STATE)
    {
        changeState(TRANSITION_FROM_CONFIG_TO_RUN_STATE);
    }
}

template <typename T>
void ApplicationStateManager<T>::handleButtonDoubleClick()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (mAppState == CONFIG_STATE)
    {
        changeState(TRANSITION_FROM_CONFIG_TO_RUN_STATE);
    }
}

template <typename T>
void ApplicationStateManager<T>::handleAllTcpClientsDisconnected()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (mAppState == TRANSITION_FROM_RUN_TO_CONFIG_STATE)
    { // if disconnected all clients we can go CONFIG STATE
        changeState(CONFIG_STATE);
    }
    else if (mAppState == TRANSITION_FROM_CONFIG_TO_RUN_STATE)
    {
        changeState(RUN_STATE);
    }
}

template <typename T>
void ApplicationStateManager<T>::handleTransitionToConfigTimeout()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (mAppState == TRANSITION_FROM_RUN_TO_CONFIG_STATE)
    {
        changeState(CONFIG_STATE);
    }
}

template <typename T>
void ApplicationStateManager<T>::handleTransitionToRunTimeout()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (mAppState == TRANSITION_FROM_CONFIG_TO_RUN_STATE)
    {
        changeState(CONFIG_STATE);
    }
}

template <typename T>
void ApplicationStateManager<T>::enterTransitionFromRunToConfigState()
{
#ifdef __DBG_APPLICATION__
    Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
    application->stopTcpServer();
    application->tcpDisconnectAllClients();
    switchToConfigTimeoutTimer.start(2000);
}

template <typename T>
void ApplicationStateManager<T>::exitTransitionFromRunToConfigState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif

    switchToConfigTimeoutTimer.stop();
    application->kikoLib.wifiDisconnectAll();
}

template <typename T>
void ApplicationStateManager<T>::enterRunState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    application->kikoLib.wifiConnectToConfigNetwork();
    application->startTcpServer();
}

template <typename T>
void ApplicationStateManager<T>::exitRunState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
}

template <typename T>
void ApplicationStateManager<T>::enterConfigState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    LedBlink(150);
    application->wifiCreateAccesPoint(AP_CONFIG_SSID, "");
    application->startTcpServer();
}

template <typename T>
void ApplicationStateManager<T>::exitConfigState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
}

template <typename T>
void ApplicationStateManager<T>::enterTransitionFromConfigToRunState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    application->stopTcpServer();
    application->tcpDisconnectAllClients();
    switchToRunTimeoutTimer.start(2000);
}

template <typename T>
void ApplicationStateManager<T>::exitTransitionFromConfigToRunState()
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    switchToRunTimeoutTimer.stop();
    application->kikoLib.wifiDisconnectAll();
}

template <typename T>
void ApplicationStateManager<T>::changeState(AppState state)
{
#ifdef __DBG_APPLICATION_STATE_MANAGER__
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
    switch (mAppState)
    {
    case RUN_STATE:
        exitRunState();
        break;
    case TRANSITION_FROM_RUN_TO_CONFIG_STATE:
        exitTransitionFromRunToConfigState();
        break;
    case CONFIG_STATE:
        exitConfigState();
        break;
    case TRANSITION_FROM_CONFIG_TO_RUN_STATE:
        exitTransitionFromConfigToRunState();
        break;
    case SWITCHING_WIFI:
        /* code */
        break;
    default:
        break;
    }

    mAppState = state;

    switch (mAppState)
    {
    case RUN_STATE:
        enterRunState();
        break;
    case TRANSITION_FROM_RUN_TO_CONFIG_STATE:
        enterTransitionFromRunToConfigState();
        break;
    case CONFIG_STATE:
        enterConfigState();
        break;
    case TRANSITION_FROM_CONFIG_TO_RUN_STATE:
        enterTransitionFromConfigToRunState();
        break;
    case SWITCHING_WIFI:
        /* code */
        break;
    default:
        break;
    }
}
