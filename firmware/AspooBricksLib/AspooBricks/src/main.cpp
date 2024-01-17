#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PseudoDNS.h>
#include <KiKoBrick.h>
#include <StateMachine.h>
#include <KiKoTimer.h>

const char *ssid = "TP-LINK_A1AE89";
const char *password = "Krzysiu1";

#define LED_PIN 2 // Pin, do którego jest podłączona dioda LED

AspooBrickT0002 mainSensor;

KiKoBrickM0002 temp;

void hadleMainSensorConnection(bool connected)
{
  Serial.printf("Handle connection function: %d\n", int(connected));
}

void handleMainSensorMeasure(float temp, float press, float humidity)
{
  Serial.printf("Handle measure function:\n");
  Serial.printf("   temperature:%f\n", temp);
  Serial.printf("   press:%f\n", press);
  Serial.printf("   humidity:%f\n", humidity);
}

StateMachine stateMachine;
KiKoTimer timer1;
KiKoTimer timer2;
KiKoTimer timer3;

class Application
{
public:
  void hadleMainSensorConnection(bool connected)
  {
    Serial.printf("Handle connection method:\n");
  }

  void handleMeasure(float temperature, float pressure, float humidity)
  {
    // static uint32_t value = 0;
    // if ((value++) % 10 == 0)
    // {
    //   Serial.printf("Handle measure method:\n");
    //   Serial.printf("   temperature:%f\n", temperature);
    //   Serial.printf("   press:%f\n", pressure);
    //   Serial.printf("   humidity:%f\n", humidity);
    // }
  }

  void handleTimer1()
  {
    // static uint32_t value = 0;
    // Serial.printf("Handle timer 1 method %d\n", value++);
  }

  void handleTimer2()
  {
    // static uint32_t value = 0;
    // Serial.printf("Handle timer 2 method %d\n", value++);
  }

  void handleTimer3()
  {
    // static uint32_t value = 0;
    // Serial.printf("Handle timer 2 method %d\n", value++);
  }
};

void handleTimer1()
{
  static uint32_t value = 0;
  Serial.printf("Handle timer 1 funcion %d\n", value++);

  if(mainSensor.isConnected()){
    mainSensor.disconnect();
  }else{
    mainSensor.begin();
  }

}

void handleTimer2()
{
  static uint32_t value = 0;
  Serial.printf("Handle timer 2 funcion %d\n", value++);
  // timer1.stop();
  // timer2.stop();
  // timer3.start(5000);
}

void handleTimer3()
{
  static uint32_t value = 0;
  Serial.printf("Handle timer 3 funcion %d\n", value++);
  // timer1.start(1000);
  // timer3.stop();
  // timer2.start(5000);
}

Application application;

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

  mainSensor.setId("T000215978085");
  mainSensor.onConnection(hadleMainSensorConnection);
  mainSensor.onMeasure(handleMainSensorMeasure);
 // mainSensor.onMeasure(&application, &Application::handleMeasure);
  

  timer1.onTimeout(handleTimer1);
  timer1.onTimeout(&application, &Application::handleTimer1);
  timer1.start(5000);

  // timer2.onTimeout(handleTimer2);
  // timer2.onTimeout(&application, &Application::handleTimer2);
  // timer2.start(250);

  // timer3.onTimeout(handleTimer3);
  // timer3.onTimeout(&application, &Application::handleTimer3);
  // timer3.start(3000);
}

void loop()
{
  KiKoBrick::update();
}