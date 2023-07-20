#include "B0002.h"

void B0002::setAutoMeasure(bool autoMeasure, uint32_t interval){
    request.mAutoMeasure = autoMeasure;
    request.autoMeasureInterval = interval;
}