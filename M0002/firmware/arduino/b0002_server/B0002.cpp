#include "B0002.h"

B0002::B0002(std::string name)
  : Brick(name), TYPE("B0002") {
}

std::string B0002::getType() const {
  return TYPE;
}

void B0002::onProtocolFrame(std::deque<uint8_t> & frame){
  Serial.println("B0002::onProtocolFrame()");
    uint8_t functionCode = frame.front();
    if (functionCode == 0x03) {
      uint8_t functionCode = ProtocolStd::getUint8_t(frame);
      float temp = ProtocolStd::getFloat(frame);
      sapi_setTemp(temp);
    } 
}

void B0002::sapi_setTemp(float temp){
  if(callbackMeasureFunction){
    callbackMeasureFunction(temp);
  }
  if(callbackMeasureMethod){
    callbackMeasureMethod(temp);
  }
}

void B0002::cmdSetMeasureInterval(bool measure, uint32_t interval_ms) {
  Serial.println("B0002::capi_setMeasureInterval");
  if (mTcpDevice) {
    std::vector<uint8_t> package;
    ProtocolStd::append(package, uint8_t(0x03));
    ProtocolStd::append(package, measure);
    ProtocolStd::append(package, interval_ms);
    mTcpDevice->sendProtocolFrame(package);
  }
}