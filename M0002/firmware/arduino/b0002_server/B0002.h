#pragma once

#include "AspooClient.h"
#include <functional>

class B0002 : public AspooClient {
public:
  void setAutoMeasure(bool autoMeasure, uint32_t interval);

  using FunctionCallback = std::function<void(AspooClient*, float)>;
  using CallbackMethod = std::function<void(AspooClient*, float)>;

  void onMeasure(FunctionCallback callback) {
    AspooClient client;
    float temperature = 30.0f;
    callback(&client, temperature);
  }

  template<typename T>
  void onMeasure(T* obj, void (T::*method)(AspooClient*, float)) {
    callbackMethod = [=](AspooClient* client, float temp) {
      (obj->*method)(client, temp);
    };
  }

  // void tick() {
  // if (callbackMethod) {
  //     if (temperature == 0.0) {
  //         // Przykładowa wartość temperatury
  //         temperature = 25.5;
  //     }
  //     callbackMethod(&sensor, temperature);
  // }

private:
  CallbackMethod callbackMethod;
  struct {
    bool mAutoMeasure = false;
    uint32_t autoMeasureInterval = 1000;
  } request;
};