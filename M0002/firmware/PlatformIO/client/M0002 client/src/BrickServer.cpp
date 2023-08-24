#include "BrickServer.hpp"

/* clients events */
void BrickServer::handleTcpError(void *arg, AsyncClient *client, int8_t error)
{
  Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

void BrickServer::handleTcpData(void *arg, AsyncClient *client, void *data, size_t len)
{
  Serial.printf("\n data received from client %s \n", client->remoteIP().toString().c_str());
  Serial.write((uint8_t *)data, len);

  std::vector<uint8_t> byteVector(static_cast<uint8_t *>(data), static_cast<uint8_t *>(data) + len);
  clients[client]->addData(byteVector);
}

void BrickServer::handleTcpDisconnect(void *arg, AsyncClient *client)
{
  Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());

  ProtocolStd * protocol = clients[client];
  protocols.erase(protocol);
  clients.erase(client);
  delete protocol;
}

void BrickServer::handleTcpTimeOut(void *arg, AsyncClient *client, uint32_t time)
{
  Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}

void BrickServer::handleProtocolStdFrame(ProtocolStd *, std::deque<uint8_t> &frame)
{
  Serial.print("Brick::handleProtocolStdFrame() ");

  if (!frame.empty())
  {
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    if (functionCode == 0x01) // ping command
    {
      std::vector<uint8_t> frame;
      ProtocolStd::append(frame, uint8_t(0x01));
      sendProtocolFrame(frame);
    }
    else if (functionCode == 0x02)
    {
      Serial.println("functionCode == 0x02");
      if (callbackGetBrickName)
      {
        callbackGetBrickName();
      }
    }
    else if (functionCode == 0x03)
    {
      Serial.println("functionCode == 0x03");
      if (callbackGetNetworkSettings)
      {
        callbackGetNetworkSettings();
      }
    }
    else if (functionCode == 0x04)
    {
      Serial.println("functionCode == 0x04");
      std::string brickName = ProtocolStd::getString(frame);
      if (callbackSaveBrickName)
      {
        callbackSaveBrickName(brickName);
      }
    }
    else if (functionCode == 0x05)
    {
      Serial.println("functionCode == 0x05");
      std::string ssid = ProtocolStd::getString(frame);
      std::string pwd = ProtocolStd::getString(frame);
      if (callbackSaveNetworkSettings)
      {
        callbackSaveNetworkSettings(ssid, pwd);
      }
    }
  }
}

void BrickServer::onHandleNewClient(void *arg, AsyncClient *client)
{
  Serial.printf("\n Brick::onHandleNewClient() prv new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());

    // register events
    client->onData(std::bind(&BrickServer::handleTcpData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), NULL);
    client->onError(std::bind(&BrickServer::handleTcpError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
    client->onDisconnect(std::bind(&BrickServer::handleTcpDisconnect, this, std::placeholders::_1, std::placeholders::_2), NULL);
    client->onTimeout(std::bind(&BrickServer::handleTcpTimeOut, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);

    ProtocolStd * protocol = new ProtocolStd();
    protocol->setOnFrameCallback(this, &BrickServer::handleProtocolStdFrame);
    clients[client] = protocol;
    protocols[protocol] = client;
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

void BrickServer::cmdSetBrickName(AsyncClient * client, const std::string &brickName)
{

  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x02));
  ProtocolStd::append(frame, getBrickType());
  ProtocolStd::append(frame, brickName);
  sendProtocolFrame(client, frame);
}

void BrickServer::cmdSetNetworkSettings(AsyncClient * client, const std::string &ssid, const std::string &pswd)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x03));
  ProtocolStd::append(frame, ssid);
  ProtocolStd::append(frame, pswd);
  sendProtocolFrame(client, frame);
}

bool BrickServer::isSomeoneConnected()
{
  return clients.size() > 0;
}

void BrickServer::update()
{
  pseudoDNS.update();
}

BrickServer::~BrickServer()
{
  delete mServer;
}

void BrickServer::sendProtocolFrame(AsyncClient *client, const std::vector<uint8_t> &frame)
{
  Serial.println("Brick::sendProtocolFrame()");
  uint16_t size = frame.size();
  std::vector<uint8_t> datagram;
  ProtocolStd::append(datagram, size);
  datagram.insert(datagram.end(), frame.begin(), frame.end());

  //  if ((mClient != nullptr) && mClient->space() > datagram.size() && mClient->canSend())
  //  {
  client->add(reinterpret_cast<const char *>(datagram.data()), datagram.size());
  Serial.println("mClient->send()");
  client->send();
  // }
}