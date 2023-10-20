#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PseudoDNS.h>
#include <AspooBricks.h>

const char *ssid = "TP-LINK_A1AE89";
const char *password = "Krzysiu1";

#define LED_PIN 2 // Pin, do którego jest podłączona dioda LED

// AspooBrickT0002 mainSensor;
PseudoDNS pseudoDNS;

void hadleMainSensorConnection(bool connected)
{
}

void handleMainSensorMeasure(float, float, float)
{
}

void handleHostFound(std::string str1, std::string str2, std::string str3, std::string str4)
{
  Serial.println("Callback ---->>");
  Serial.println(str1.c_str());
  Serial.println(str2.c_str());
  Serial.println(str3.c_str());
  Serial.println(str4.c_str());
}

void setup()
{
  delay(100);
  Serial.begin(3000000);
  Serial.println("Start Aspoo Brick Library");

  // Łączenie z siecią Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Łączenie z siecią Wi-Fi...");
  }

  pinMode(LED_PIN, OUTPUT); // Ustaw pin jako wyjście

  // mainSensor.setId("T000215978085");
  // mainSensor.onConnection(hadleMainSensorConnection);
  // mainSensor.onMeasure(handleMainSensorMeasure);
  // mainSensor.begin();

  pseudoDNS.onHostFound(handleHostFound);
  pseudoDNS.startQueriesForAllHosts();
}

void loop()
{
  // digitalWrite(LED_PIN, HIGH); // Włącz diodę LED (stan wysoki)
  // delay(1000);                 // Poczekaj przez 1 sekundę

  // digitalWrite(LED_PIN, LOW); // Wyłącz diodę LED (stan niski)
  // delay(1000);                // Poczekaj przez 1 sekundę

  // // AspooBrick::update();

  // Serial.println("Start Aspoo Brick Library");
  pseudoDNS.update();

  Serial.println(pseudoDNS.getIPById("T000215978085").c_str());

  delay(1000);    
}