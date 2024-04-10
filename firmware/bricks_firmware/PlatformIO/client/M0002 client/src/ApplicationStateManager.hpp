#pragma once
#include "config.h"
#include "Timer.hpp"

template <typename T>
class Application;

template <typename T>
class ApplicationStateManager
{
public:
    enum AppState
    {
        RUN_STATE,
        TRANSITION_FROM_RUN_TO_CONFIG_STATE,
        CONFIG_STATE,
        TRANSITION_FROM_CONFIG_TO_RUN_STATE,
        SWITCHING_WIFI
    };
    ApplicationStateManager(Application<T> *application);
    void changeState(AppState state);
    void handleButtonLongPress();
    void handleButtonClick();
    void handleButtonDoubleClick();
    void handleAllTcpClientsDisconnected();

    AppState mAppState = RUN_STATE;

private:
    void enterRunState();
    void exitRunState();
    void enterTransitionFromRunToConfigState();
    void exitTransitionFromRunToConfigState();
    void enterConfigState();
    void exitConfigState();
    void enterTransitionFromConfigToRunState();
    void exitTransitionFromConfigToRunState();

    void handleTransitionToConfigTimeout();
    void handleTransitionToRunTimeout();

private:
    Application<T> *application;
    Timer switchToConfigTimeoutTimer;
    Timer switchToRunTimeoutTimer;
};