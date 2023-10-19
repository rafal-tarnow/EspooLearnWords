#include "BrickClient.hpp"

uint32_t BrickClient::instancesCreated = 0;
uint32_t BrickClient::disconnections = 0;
uint32_t BrickClient::errorsCount = 0;
uint32_t BrickClient::timeouts = 0;
uint64_t BrickClient::bytesRx = 0;
std::vector<std::string> BrickClient::errors;
std::unordered_set<BrickClient *> BrickClient::waitingClients;

BrickClient::BrickClient(AsyncClient *tcpClient)
{
  instancesCreated++;
  asyncClient = tcpClient;
  protocol.setOnFrameCallback(this, &BrickClient::handleProtocolStdFrame);

  // asyncClient->setNoDelay(true);
  asyncClient->onData(std::bind(&BrickClient::handleTcpData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), NULL);
  asyncClient->onError(std::bind(&BrickClient::handleTcpError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
  asyncClient->onDisconnect(std::bind(&BrickClient::handleTcpDisconnect, this, std::placeholders::_1, std::placeholders::_2), NULL);
  asyncClient->onTimeout(std::bind(&BrickClient::handleTcpTimeOut, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
}

BrickClient::BrickClient(TcpSocket *socket)
{
  instancesCreated++;
  tcpSocket = socket;
  protocol.setOnFrameCallback(this, &BrickClient::handleProtocolStdFrame);

  tcpSocket->onData(this, &BrickClient::handleTcpSocketData);
  tcpSocket->onDisconnected(this, &BrickClient::handleTcpSocketDisconnect);
  tcpSocket->onError(this, &BrickClient::handleTcpSocketError);
}

BrickClient::~BrickClient()
{
  // if(asyncClient)
  waitingClients.erase(this);
  delete asyncClient;
  // if(tcpSocket)
  //   delete tcpSocket;
}

void BrickClient::handleTcpData(void *arg, AsyncClient *client, void *data, size_t len)
{
  // Serial.printf("\n%lu BrickClient::handleTcpData()", dtime());
  bytesRx += len;
  std::vector<uint8_t> byteVector(static_cast<uint8_t *>(data), static_cast<uint8_t *>(data) + len);
  protocol.addData(byteVector);
}

void BrickClient::handleTcpError(void *arg, AsyncClient *asyncClient, int8_t error)
{
  Serial.printf("\n%lu BrickClient::handleTcpError() from client %s \n", dtime(), asyncClient->remoteIP().toString().c_str());
  errorsCount++;
  // errors.push_back(std::string(asyncClient->errorToString(error)));
}

void BrickClient::handleTcpDisconnect(void *arg, AsyncClient *asyncClient)
{
  Serial.printf("\n BrickClient::handleTcpDisconnect() %s disconnected \n", asyncClient->remoteIP().toString().c_str());
  disconnections++;
  bufferToSend.clear();
  waitingClients.erase(this);
  if (callbackTcpDisconnected)
  {
    callbackTcpDisconnected(this);
  }
}

void BrickClient::handleTcpSocketDisconnect()
{
  Serial.printf("\n%lu BrickClient::handleTcpSocketDisconnect():", dtime());
  disconnections++;
  if (callbackTcpDisconnected)
  {
    callbackTcpDisconnected(this);
  }
}

void BrickClient::handleTcpSocketError(const std::string &error)
{
  Serial.printf("\n%lu BrickClient::handleTcpError() from client %s \n", dtime(), asyncClient->remoteIP().toString().c_str());
  // errorsCount++;
  // errors.push_back(error);
  // ESP.reset();
}

void BrickClient::handleTcpTimeOut(void *arg, AsyncClient *client, uint32_t time)
{
  Serial.printf("\n BrickClient::handleTcpTimeOut() client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
  timeouts++;
}

void BrickClient::handleTcpSocketData(const uint8_t *buffer, uint16_t len)
{
  Serial.printf("\n%lu BrickClient::handleSocketData():", dtime());
  // Tutaj możesz przetwarzać i wyświetlać odebrane dane
  std::vector<uint8_t> byteVector(buffer, buffer + len);
  protocol.addData(byteVector);
}

void BrickClient::printDbg()
{
  Serial.printf("\n%lu BrickClient::printDbg():", dtime());
  Serial.printf("\n%lu        instancesCreated: %d", dtime(), instancesCreated);
  Serial.printf("\n%lu        disconnections: %d", dtime(), disconnections);
  Serial.printf("\n%lu        errors: %d", dtime(), errorsCount);
  Serial.printf("\n%lu        timeouts: %d", dtime(), timeouts);
  Serial.printf("\n%lu        bytesRx: %llu", dtime(), bytesRx);
  // for (auto error : errors)
  // {
  //   Serial.printf("\n%lu        error: %s", dtime(), error.c_str());
  // }
}

size_t BrickClient::space()
{
  if (asyncClient)
    return asyncClient->space();
  return 0;
}

bool BrickClient::isAnyWaiting()
{
  //Serial.printf("\n waitingClients = %d", waitingClients.size());
  return waitingClients.size() > 0 ? true : false;
}

void BrickClient::tryFlushBuffers()
{
  std::unordered_set<BrickClient *> flushedClients;
  Serial.printf("\nBrickClient::FLUSH() 1");
  for (BrickClient *client : waitingClients)
  {
    Serial.printf("\nBrickClient::FLUSH() 2");
    if (client->tryFlushBuffer())
    {
      flushedClients.insert(client);
    }
    Serial.printf("\nBrickClient::FLUSH() 3");
  }
  Serial.printf("\nBrickClient::FLUSH() 4");

  for (BrickClient *client : flushedClients)
  {
    waitingClients.erase(client);
  }
}

void BrickClient::handleProtocolStdFrame(std::deque<uint8_t> &frame)
{
  // Serial.printf("\n%lu BrickClient::handleProtocolStdFrame() ", dtime());

  if (!frame.empty())
  {
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    // Serial.printf("\n%lu functionCode == %d", dtime(), functionCode);
    if (functionCode == 0x01) // ping command
    {
      // Serial.println("\nfunctionCode == 0x01");
      std::vector<uint8_t> frame;
      ProtocolStd::append(frame, uint8_t(0x01));
      sendProtocolFrame(frame);
    }
    else if (functionCode == 0x02)
    {
      Serial.printf("function code = 0x02 getInfo()");
      if (callbackGetInfo)
      {
        callbackGetInfo(this);
      }
    }
    else if (functionCode == 0x03)
    {
      if (callbackGetId)
      {
        callbackGetId(this);
      }
    }
    else if (functionCode == 0x04)
    {
      if (callbackGetType)
      {
        callbackGetType(this);
      }
    }
    else if (functionCode == 0x05)
    {
      if (callbackGetBrickName)
      {
        callbackGetBrickName(this);
      }
    }
    else if (functionCode == 0x06)
    {
      if (callbackGetNetworkSettings)
      {
        callbackGetNetworkSettings(this);
      }
    }
    else if (functionCode == 0x07)
    {
      std::string brickName = ProtocolStd::getString(frame);
      if (callbackSaveBrickName)
      {
        callbackSaveBrickName(this, brickName);
      }
    }
    else if (functionCode == 0x08)
    {
      std::string ssid = ProtocolStd::getString(frame);
      std::string pwd = ProtocolStd::getString(frame);
      if (callbackSaveNetworkSettings)
      {
        callbackSaveNetworkSettings(this, ssid, pwd);
      }
    }
  }
}

bool BrickClient::isWaiting(BrickClient *client)
{
  return waitingClients.find(client) != waitingClients.end();
}

bool BrickClient::tryFlushBuffer()
{
  Serial.printf("\nBrickClient::tryFlushBuffer()");
  if ((asyncClient != nullptr) && (asyncClient->connected()))
  {
    Serial.printf("\nBrickClient::tryFlushBuffer() 1");
    if (asyncClient->space() >= bufferToSend.size())
    {
      size_t addedSize = asyncClient->add(reinterpret_cast<const char *>(bufferToSend.data()), bufferToSend.size());
      asyncClient->send();

      bufferToSend.clear();
      return true;
    }
  }
  Serial.printf("\nBrickClient::tryFlushBuffer() 7");
  return false;
}

void BrickClient::cmdSetInfo(const std::string& id, const std::string &brickType, const std::string &brickName, const std::string &ssid, const std::string &pswd)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x02));
  ProtocolStd::append(frame, id);
  ProtocolStd::append(frame, brickType);
  ProtocolStd::append(frame, brickName);
  ProtocolStd::append(frame, ssid);
  ProtocolStd::append(frame, pswd);

  sendProtocolFrame(frame);
}

void BrickClient::cmdSetId(const std::string& id)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x03));
  ProtocolStd::append(frame, id);
  sendProtocolFrame(frame);
}

void BrickClient::cmdSetType(const std::string &brickType)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x04));
  ProtocolStd::append(frame, brickType);
  sendProtocolFrame(frame);
}

void BrickClient::cmdSetName(const std::string &brickName)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x05));
  ProtocolStd::append(frame, brickName);
  sendProtocolFrame(frame);
}

void BrickClient::cmdSetNetworkSettings(const std::string &ssid, const std::string &pswd)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x06));
  ProtocolStd::append(frame, ssid);
  ProtocolStd::append(frame, pswd);
  sendProtocolFrame(frame);
}

void BrickClient::sendProtocolFrame(const std::vector<uint8_t> &frame)
{
  // Serial.printf("\n%lu Brick::sendProtocolFrame() ", dtime());
  uint16_t size = frame.size();
  std::vector<uint8_t> datagram;
  ProtocolStd::append(datagram, size);
  datagram.insert(datagram.end(), frame.begin(), frame.end());

  if (tcpSocket != nullptr)
  {
    tcpSocket->write(reinterpret_cast<const uint8_t *>(datagram.data()), datagram.size());
    return;
  }

//  if ((mClient != nullptr) && mClient->space() > datagram.size() && mClient->canSend())
//  {
#warning "TODO rapair this"
  if ((asyncClient != nullptr) && (asyncClient->connected()))
  {

    if (!isWaiting(this) && asyncClient->space() >= datagram.size())
    {

      size_t addedSize = asyncClient->add(reinterpret_cast<const char *>(datagram.data()), datagram.size());
      asyncClient->send();

      for (const uint8_t &value : datagram)
      {
        Serial.print(value);
        Serial.print(" ");
      }
      Serial.println(); // Nowa linia na końcu
    }
    else
    {
      bufferToSend.insert(bufferToSend.end(), datagram.begin(), datagram.end());
      Serial.printf("\n%lu Buffer to send size() = %d", dtime(), bufferToSend.size());
      waitingClients.insert(this);
    }
  }

  // if (!isWaiting(this) && asyncClient->space() <= datagram.size())
  // {
  //   size_t addedSize = asyncClient->add(reinterpret_cast<const char *>(datagram.data()), datagram.size());
  //   asyncClient->send();
  //   Serial.printf("\n%lu mClient->send() %d bytes", dtime(), addedSize);
  //   Serial.printf("\n datagram.size() = %d ", datagram.size());
  // tutaj reszte z danych z datagrama ktor sie nie zmienściły w add chce dodata do bufferToSend, jak to zrobić??
  // tu mi chcacie gtp zaimplementuj tovoid brickTypeAndName(const QString & type, const QString & brickName);
  //     bufferToSend.insert(bufferToSend.end(), datagram.begin() + addedSize, datagram.end());
  //     Serial.printf("\n%lu 3", dtime());
  //     waitingClients.insert(this);
  //     Serial.printf("\n%lu 4", dtime());
  //   }
  // }
  // else
  // { // not enought space to add frame
  //   Serial.printf("\n%lu 5", dtime());
  //   bufferToSend.insert(bufferToSend.end(), datagram.begin(), datagram.end());
  //   Serial.printf("\n%lu 6", dtime());
  //   waitingClients.insert(this);
  //   Serial.printf("\n%lu 7", dtime());
  // }
  // }
  // else
  // {
  //   Serial.printf("\n%lu 8", dtime());
  //   Serial.printf("\n%lu asyncClient->connected() donnt connected", dtime());
  //   // ESP.reset();
  // }
  // Serial.printf("\n%lu 2", dtime());
}
// }
