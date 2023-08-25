#include "BrickServer.hpp"

/* clients events */

void BrickServer::handleTcpData(void *arg, AsyncClient *asyncClient, void *data, size_t len)
{
  brickClients[asyncClient]->handleTcpData(arg, asyncClient, data, len);
}

void BrickServer::handleTcpError(void *arg, AsyncClient *asyncClient, int8_t error)
{
  Serial.printf("\n connection error %s from client %s \n", asyncClient->errorToString(error), asyncClient->remoteIP().toString().c_str());
}

void BrickServer::handleTcpDisconnect(void *arg, AsyncClient *asyncClient)
{
  Serial.printf("\n BrickServer::handleTcpDisconnect() %s disconnected \n", asyncClient->remoteIP().toString().c_str());

if(callbackBrickDelete){
  BrickClient *brickClient = brickClients[asyncClient];
  Serial.printf("\n brickClient->handleTcpDisconnect");
  brickClient->handleTcpDisconnect(arg, asyncClient);
  Serial.printf("\nbrickClients.erase()");
  brickClients.erase(asyncClient);
  Serial.printf("\n delete");
  callbackBrickDelete(brickClient);
  Serial.printf("\after delete");
  
}
delete asyncClient;
}

void BrickServer::handleTcpTimeOut(void *arg, AsyncClient *client, uint32_t time)
{
  Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}

void BrickServer::onHandleNewClient(void *arg, AsyncClient *asyncClient)
{
  Serial.printf("\n BrickServer::onHandleNewClient() prv new client has been connected to server, ip: %s", asyncClient->remoteIP().toString().c_str());

  Serial.printf("\nCreate new BrickClient");
  if (callbackBrickCreate)
  {
    Serial.printf("\nInside new BrickClient");
    BrickClient *newBrickClient = callbackBrickCreate(asyncClient);
    brickClients[asyncClient] = newBrickClient;

    // register events
    asyncClient->onData(std::bind(&BrickServer::handleTcpData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), NULL);
    asyncClient->onError(std::bind(&BrickServer::handleTcpError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
    asyncClient->onDisconnect(std::bind(&BrickServer::handleTcpDisconnect, this, std::placeholders::_1, std::placeholders::_2), NULL);
    asyncClient->onTimeout(std::bind(&BrickServer::handleTcpTimeOut, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
  }
  Serial.printf("\nAfeter Create new BrickClient");
}

void BrickServer::begin(const std::string &brickName)
{
  pseudoDNS.run(brickName);

  mServer = new AsyncServer(PORT); // start listening on tcp port 7050
  mServer->onClient(std::bind(&BrickServer::onHandleNewClient, this, std::placeholders::_1, std::placeholders::_2), mServer);
  mServer->begin();
}

void BrickServer::setBrickName(const std::string &brickName)
{
  pseudoDNS.setHostName(brickName);
}

bool BrickServer::isSomeoneConnected()
{
  return brickClients.size() > 0;
}

void BrickServer::update()
{
  pseudoDNS.update();
}

BrickServer::~BrickServer()
{
  delete mServer;
}
