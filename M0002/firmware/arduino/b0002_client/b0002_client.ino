// #include <WiFi.h>
// #include <PubSubClient.h>


// // Replace the next variables with your SSID/Password combination
// const char* ssid = "AndroidAP";
// const char* password = "wicl3264";

// // Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.135.209";
// //const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";

// WiFiClient espClient;
// PubSubClient client(espClient);
// long lastMsg = 0;
// char msg[50];
// int value = 0;

// //uncomment the following lines if you're using SPI
// /*#include <SPI.h>
// #define BME_SCK 18
// #define BME_MISO 19
// #define BME_MOSI 23
// #define BME_CS 5*/

// Adafruit_BME280 bme; // I2C
// //Adafruit_BME280 bme(BME_CS); // hardware SPI
// //Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
// float temperature = 0;
// float humidity = 0;

// // LED Pin
// const int ledPin = 4;

// void setup() {
//   Serial.begin(115200);
//   // default settings
//   // (you can also pass in a Wire library object like &Wire2)
//   //status = bme.begin();  
//   if (!bme.begin(0x76)) {
//     Serial.println("Could not find a valid BME280 sensor, check wiring!");
//     while (1);
//   }
//   setup_wifi();
//   client.setServer(mqtt_server, 1883);
//   client.setCallback(callback);

//   pinMode(ledPin, OUTPUT);
// }

// void setup_wifi() {
//   delay(100);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void callback(char* topic, byte* message, unsigned int length) {
//   Serial.print("Message arrived on topic: ");
//   Serial.print(topic);
//   Serial.print(". Message: ");
//   String messageTemp;
  
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)message[i]);
//     messageTemp += (char)message[i];
//   }
//   Serial.println();

//   // Feel free to add more if statements to control more GPIOs with MQTT

//   // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
//   // Changes the output state according to the message
//   if (String(topic) == "esp32/output") {
//     Serial.print("Changing output to ");
//     if(messageTemp == "on"){
//       Serial.println("on");
//       digitalWrite(ledPin, HIGH);
//     }
//     else if(messageTemp == "off"){
//       Serial.println("off");
//       digitalWrite(ledPin, LOW);
//     }
//   }
// }

// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Attempt to connect
//     if (client.connect("ESP8266Client")) {
//       Serial.println("connected");
//       // Subscribe
//       client.subscribe("esp32/output");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }
// void loop() {
//   if (!client.connected()) {
//     reconnect();
//   }
//   client.loop();

//   long now = millis();
//   if (now - lastMsg > 5000) {
//     lastMsg = now;
    
//     // Temperature in Celsius
//     temperature = bme.readTemperature();   
//     // Uncomment the next line to set temperature in Fahrenheit 
//     // (and comment the previous temperature line)
//     //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
//     // Convert the value to a char array
//     char tempString[8];
//     dtostrf(temperature, 1, 2, tempString);
//     Serial.print("Temperature: ");
//     Serial.println(tempString);
//     client.publish("esp32/temperature", tempString);

//     humidity = bme.readHumidity();
    
//     // Convert the value to a char array
//     char humString[8];
//     dtostrf(humidity, 1, 2, humString);
//     Serial.print("Humidity: ");
//     Serial.println(humString);
//     client.publish("esp32/humidity", humString);
//   }
// }









/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "AA:BB:CC" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>

// // Update these with values suitable for your network.

// static const char* ssid = "AndroidAP";
// static const char* password = "wicl3264";
// static const char* mqtt_server = "broker.hivemq.com";

// WiFiClient espClient;
// PubSubClient client(espClient);
// unsigned long lastMsg = 0;
// #define MSG_BUFFER_SIZE	(50)
// char msg[MSG_BUFFER_SIZE];
// int value = 0;

// void setup_wifi() {

//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   randomSeed(micros());

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();

//   // Switch on the LED if an 1 was received as first character
//   if ((char)payload[0] == '1') {
//     digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
//     // but actually the LED is on; this is because
//     // it is active low on the ESP-01)
//   } else {
//     digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
//   }

// }

// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Create a random client ID
//     String clientId = "ESP8266Client-";
//     clientId += String(random(0xffff), HEX);
//     // Attempt to connect
//     if (client.connect(clientId.c_str())) {
//       Serial.println("connected");
//       // Once connected, publish an announcement...
//       client.publish("AA:BB:CC", "hello world");
//       // ... and resubscribe
//       client.subscribe("inTopic");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// void setup() {
//   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
//   Serial.begin(115200);
//   setup_wifi();
//   client.setServer(mqtt_server, 1883);
//   client.setCallback(callback);
// }

// void loop() {

//   if (!client.connected()) {
//     reconnect();
//   }
//   client.loop();

//   unsigned long now = millis();
//   if (now - lastMsg > 2000) {
//     lastMsg = now;
//     ++value;
//     snprintf (msg, MSG_BUFFER_SIZE, "Temp kuchni=23;Wilgotnosc=50", value);
//     Serial.print("Publish message: ");
//     Serial.println(msg);
//     client.publish("AA:BB:CC", msg);
//   }
// }

// // Definicje pinów
// const int buttonPin = 0;  // Przycisk podłączony do pinu GPIO0
// const int ledPin = 2;     // Dioda LED podłączona do pinu GPIO2 (D4)

// // Zmienne
// int buttonState = 0;

// void setup() {
//   // Inicjalizacja pinów
//   pinMode(buttonPin, INPUT);  // Ustawienie przycisku jako wejście z wewnętrznym podciąganiem do Vcc
//   pinMode(ledPin, OUTPUT);            // Ustawienie diody LED jako wyjście
// }

// void loop() {
//   // Odczytaj stan przycisku
//   buttonState = digitalRead(buttonPin);

//   // Jeśli przycisk jest wciśnięty (pin zwarty do masy), włącz diodę LED
//   if (buttonState == LOW) {
//     digitalWrite(ledPin, HIGH);  // Włącz diodę LED
//   } else {
//     digitalWrite(ledPin, LOW);   // Wyłącz diodę LED
//   }
// }

// Include the libraries we need
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "OneButton.h"
#include "application.h"
#include <memory>

extern "C" {
#include <osapi.h>
#include <os_type.h>
}


#define ONE_WIRE_BUS 4
const int ledPin = 2; 
#define BUTTON_PIN 0

static os_timer_t intervalTimer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

OneButton button = OneButton(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  false        // Enable internal pull-up resistor
);

std::unique_ptr<Application> application;

//#define SSID "TP-LINK_A1AE89"
#define SSID "DupaNetwork"
#define PASSWORD "Krzysiu1"
#define SERVER_HOST_NAME "Aspoo_server"
#define TCP_PORT 7050

void longPress();
void click();

static void replyToServer(void* arg) {
	AsyncClient* client = reinterpret_cast<AsyncClient*>(arg);

	// send reply
	if (client->space() > 32 && client->canSend()) {
		char message[32];
		sprintf(message, "this is from %s", WiFi.localIP().toString().c_str());
		client->add(message, strlen(message));
		client->send();
	}
}

/* event callbacks */
static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
	Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
	Serial.write((uint8_t*)data, len);

	os_timer_arm(&intervalTimer, 2000, true); // schedule for reply to server at next 2s
}

void onConnect(void* arg, AsyncClient* client) {
	Serial.printf("\n client has been connected to %s on port %d \n", SERVER_HOST_NAME, TCP_PORT);
	replyToServer(client);
}

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(115200);
  Serial.println("B0002 client");

  // Start up the library
  sensors.begin();

  //button.attachLongPressStop(longPress);
  button.attachLongPressStart(longPress);
  button.attachClick(click);
  button.setPressMs(7000);

  pinMode(ledPin, OUTPUT); 

  application = std::make_unique<Application>();

  // connects to access point
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(500);
	}
  Serial.print("Connected to WiFi Network! IP address: ");
  Serial.println(WiFi.localIP());

  AsyncClient* client = new AsyncClient;
	client->onData(&handleData, client);
	client->onConnect(&onConnect, client);
	client->connect(SERVER_HOST_NAME, TCP_PORT);

  os_timer_disarm(&intervalTimer);
	os_timer_setfn(&intervalTimer, &replyToServer, client);
}


/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  // Serial.print("Requesting temperatures...");
  // sensors.requestTemperatures(); // Send the command to get temperatures
  // Serial.println("DONE");
  // // After we got the temperatures, we can print them here.
  // // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  // float tempC = sensors.getTempCByIndex(0);

  // // Check if reading was successful
  // if(tempC != DEVICE_DISCONNECTED_C) 
  // {
  //   Serial.print("Temperature for the device 1 (index 0) is: ");
  //   Serial.println(tempC);
  // } 
  // else
  // {
  //   Serial.println("Error: Could not read temperature data");
  // }

  // delay(1500);
  button.tick();
}

void click(){
  application->onButtonPressedEvent();
}

void longPress() {
  static bool state = false;
  state = !state;
  digitalWrite(ledPin, state);
  application->onButtonLongPressedEvent();
}








