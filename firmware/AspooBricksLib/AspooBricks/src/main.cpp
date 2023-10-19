#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PseudoDNS.h>
#include <AspooBricks.h>

#define LED_PIN 2 // Pin, do którego jest podłączona dioda LED


AspooBrickT0002 mainSensor;

void hadleMainSensorConnection(bool connected){

}

void handleMainSensorMeasure(float, float, float){

}

void setup() {
  pinMode(LED_PIN, OUTPUT); // Ustaw pin jako wyjście

  mainSensor.setId("T0002id");
  mainSensor.onConnection(hadleMainSensorConnection);
  mainSensor.onMeasure(handleMainSensorMeasure);
  mainSensor.begin();

}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Włącz diodę LED (stan wysoki)
  delay(1000); // Poczekaj przez 1 sekundę

  digitalWrite(LED_PIN, LOW); // Wyłącz diodę LED (stan niski)
  delay(1000); // Poczekaj przez 1 sekundę

  AspooBrick::update();
}