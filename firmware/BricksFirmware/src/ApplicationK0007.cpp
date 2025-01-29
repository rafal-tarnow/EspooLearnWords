#include "ApplicationK0007.hpp"

ApplicationK0007::ApplicationK0007() : button()
{
}

void ApplicationK0007::setup()
{
    button.attach(BUTTON_PIN, INPUT);
    button.interval(5);
    button.setPressedState(HIGH);
}

void ApplicationK0007::loop()
{
    // #ifdef __DBG_APPLICATION__K0007__
    //     Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
    // #endif
    button.update();

    bool buttonState = button.read();
    if (mSwitchState != buttonState)
    {
        buttonChanges++;
        mSwitchState = buttonState;
        Serial.printf("Switch state changed %d\n", !mSwitchState);
        Serial.printf("Button changes %d\n", buttonChanges);

        for (auto it = kikoLib.clients.begin(); it != kikoLib.clients.end(); ++it)
        {
            K0007 *ptr = *it;
            ptr->cmdSetSwitchState(!mSwitchState);
        }
    }
}

void ApplicationK0007::handleNewBrickClient(K0007 *client)
{
}
