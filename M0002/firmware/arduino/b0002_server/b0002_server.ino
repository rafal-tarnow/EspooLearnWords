#include <ESP8266WiFi.h>
#include <vector>
#include "Application.h"
#include "AspooServer.h"

//#define SSID "TP-LINK_A1AE89"
#define SSID "DupaNetwork"
#define PASSWORD "12345678"

#define SERVER_HOST_NAME "AspooServer"
#define DNS_PORT 53
#define TCP_PORT 7050

static void handleMeasureRoom(Brick* brick, float temp) {
	
}

Application application;

AspooServer server("AspooServer");
B0002 tempRoom("tempRoom");
B0002 tempOutdoor("tempOutdoor");
B0002 tempBoiler("tempBoiler");


static void connectedTempRoom() {
	Serial.println("connectedTempRoom()-");
  tempRoom.cmdSetMeasureInterval(true, 1000);
}

static void disconnectedTempRoom() {
	Serial.println("disconnectedTempRoom()");
}

static void onMeasureRoom(float temp){
  Serial.println("onMeasureRoom()");
  Serial.println(temp);
}





void setup(){
  Serial.begin(115200);
  delay(200);
  Serial.printf("\n----- START ASPOO SERVER -----");

  server.begin();

  tempRoom.onConnected(connectedTempRoom);
  tempRoom.onDisconnected(disconnectedTempRoom);
  tempRoom.onMeasure(onMeasureRoom);

  tempBoiler.onConnected(&application, &Application::onConnectedTempBoiler);
  tempBoiler.onDisconnected(&application, &Application::onDisconnectedTempBoiler);
  tempBoiler.onMeasure(&application, &Application::onnMeasureBoiler);

  	// connects to access point
	WiFi.mode(WIFI_STA);
	WiFi.begin("TP-LINK_A1AE89", "Krzysiu1");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(500);
	}
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  	// create access point
	// while (!WiFi.softAP(SSID, PASSWORD, 6, false, 15)) {
	// 	delay(500);
	// }
}

void loop(){
  server.update();
}