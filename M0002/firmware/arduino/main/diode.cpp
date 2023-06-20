// #include "diode.h"
// #include <Arduino.h>
// #include <Preferences.h>


// static bool onOffState = false;
// static Preferences preferences;

// void setOnOffWithoutPrefs(bool state);

// void initOnOff() {
//   pinMode(LED_BUILTIN, OUTPUT);

//   preferences.begin("M0001", false);
//   onOffState = preferences.getBool("OnOff", 1);
//   preferences.end();

//   setOnOffWithoutPrefs(onOffState);
// }

// bool getOnOffState(){
//   return onOffState;
// }

// void setOnOff(bool state) {
//   onOffState = state;
//   if (state) {
//     digitalWrite(LED_BUILTIN, LOW);
//   } else {
//     digitalWrite(LED_BUILTIN, HIGH);
//   }
//   preferences.begin("M0001", false);
//   preferences.putBool("OnOff", state);
//   preferences.end();
// }

// void setOnOffWithoutPrefs(bool state) {
//   onOffState = state;
//   if (state) {
//     digitalWrite(LED_BUILTIN, LOW);
//   } else {
//     digitalWrite(LED_BUILTIN, HIGH);
//   }
// }