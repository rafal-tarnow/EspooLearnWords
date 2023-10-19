#include "Application.h"


void Application::onConnectedTempBoiler() {
  Serial.println("Application::onConnectedTempBoiler()");
}

void Application::onDisconnectedTempBoiler() {
  Serial.println("Application::onDisconnectedTempBoiler()");
}

void Application::onnMeasureBoiler(float temp) {
  Serial.println("Application::onMeasureBoiler()");
  Serial.println(temp);
}