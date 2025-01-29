#include "time.h"
#include <Arduino.h>

static unsigned long prevTime = 0;
static unsigned long deltaTime = 0;
static unsigned long currentTime = 0;

unsigned long dtime(){
    currentTime = millis();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    return deltaTime;
}

// unsigned long dtime(){ 
//     return millis();
// }