#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <vector>
#include "Application.h"
#include "AspooServer.h"

//#define SSID "TP-LINK_A1AE89"
#define SSID "DupaNetwork"
#define PASSWORD "Krzysiu1"

#define SERVER_HOST_NAME "AspooServer"
#define DNS_PORT 53
#define TCP_PORT 7050

static std::vector<AsyncClient*> clients; // a list to hold all clients

 /* clients events */
static void handleError(void* arg, AsyncClient* client, int8_t error) {
	Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
	Serial.printf("\n data received from client %s \n", client->remoteIP().toString().c_str());
	Serial.write((uint8_t*)data, len);

	// reply to client
	if (client->space() > 32 && client->canSend()) {
		char reply[32];
		sprintf(reply, "this is from %s", SERVER_HOST_NAME);
		client->add(reply, strlen(reply));
		client->send();
	}
}

static void handleDisconnect(void* arg, AsyncClient* client) {
	Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
	Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}

/* server events */
static void handleNewClient(void* arg, AsyncClient* client) {
	Serial.printf("\n New client has been connected to server, ip: %s", client->remoteIP().toString().c_str());
  Serial.printf("\n     client Ack timeout: %d", client->getAckTimeout());

	// add to list
	clients.push_back(client);
	
	// register events
	client->onData(&handleData, NULL);
	client->onError(&handleError, NULL);
	client->onDisconnect(&handleDisconnect, NULL);
	client->onTimeout(&handleTimeOut, NULL);
  //client->onAck(AcAckHandler cb)
}

static void handleMeasure(AspooClient* client, float temp) {
	
}

Application application;

AspooServer server;
B0002 tempRoom;
B0002 tempOutdoor;
B0002 tempBoiler;

void setup(){
  Serial.begin(115200);
  delay(20);
  Serial.printf("\n----- START ASPOO SERVER -----");


  server.begin("AspooServerĆŚąęĄĘ");

  tempRoom.setName("tempRoom");
  tempRoom.setAutoMeasure(true, 1000);
  tempRoom.onMeasure(&handleMeasure);

  tempOutdoor.setName("tempOutdoor");
  tempRoom.setAutoMeasure(true, 1000);
  tempRoom.onMeasure(&handleMeasure);

  tempBoiler.setName("tempBoiler");
  tempRoom.setAutoMeasure(true, 1000);


  // 	// connects to access point
	WiFi.mode(WIFI_STA);
	WiFi.begin("TP-LINK_A1AE89", "Krzysiu1");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(500);
	}
  // Serial.print("Connected! IP address: ");
  // Serial.println(WiFi.localIP());

  	// create access point
	// while (!WiFi.softAP(SSID, PASSWORD, 6, false, 15)) {
	// 	delay(500);
	// }


  AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
	server->onClient(&handleNewClient, server);
	server->begin();
}

void loop(){
  server.update();
}