#pragma once

#include "Brick.h"
#include <functional>
#include <string>

class B0002 : public Brick {
  using CallbackMeasureFunc = std::function<void(float)>;
  using CallbackMeasureMeth = std::function<void(float)>;
public:
  B0002(std::string name);
  void onMeasure(CallbackMeasureFunc callback) {
    callbackMeasureFunction = callback;
  }
  template<typename T>
  void onMeasure(T *obj, void (T::*method)(float temp)) {
    callbackMeasureMethod = [=](float temp) {
      (obj->*method)(temp);
    };
  }
  void cmdSetMeasureInterval(bool measure, uint32_t interval_ms);

protected:
  std::string getType() const override;
  void onProtocolFrame(std::deque<uint8_t> &) override;

private:
  void sapi_setTemp(float temp);
  std::string TYPE;
  CallbackMeasureFunc callbackMeasureFunction;
  CallbackMeasureMeth callbackMeasureMethod;
};