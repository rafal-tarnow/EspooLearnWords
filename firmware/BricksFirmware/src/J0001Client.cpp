#include "J0001Client.hpp"

J0001Client::J0001Client(AsyncClient *asyncClient) : BrickClient(asyncClient)
{
}

J0001Client::J0001Client(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void J0001Client::cmdSetRun(bool run)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x11));
  ProtocolStd::append(frame, run);
  sendProtocolFrame(frame);
}

void J0001Client::cmdSetDropletVolume(float dropletVolume)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x12));
  ProtocolStd::append(frame, dropletVolume);
  sendProtocolFrame(frame);
}

void J0001Client::cmdSetWindowDuration(uint8_t windowDuration)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x13));
  ProtocolStd::append(frame, windowDuration);
  sendProtocolFrame(frame);
}

void J0001Client::cmdSetMeasureValues(uint64_t counter, float milliliters, float flowRate)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x14));
  ProtocolStd::append(frame, counter);
  ProtocolStd::append(frame, milliliters);
  ProtocolStd::append(frame, flowRate);
  sendProtocolFrame(frame);
}

void J0001Client::cmdSetRuntime(uint32_t seconds)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x15));
  ProtocolStd::append(frame, seconds);
  sendProtocolFrame(frame);
}

void J0001Client::cmdSetMaxFlowDisplay(float maxFlowDisplay)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x16));
  ProtocolStd::append(frame, maxFlowDisplay);
  sendProtocolFrame(frame);
}

std::string J0001Client::type()
{
  return "J0001";
}

void J0001Client::processProtocolStdFrame(std::deque<uint8_t> &frame)
{
#ifdef __DBG_J0001__
  Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
  uint8_t functionCode = ProtocolStd::popUint8_t(frame);
  if (functionCode == 0x10)
  {
    bool runState = ProtocolStd::popBool(frame);
#ifdef __DBG_J0001__
    Serial.printf("runState=%d\n", runState);
#endif
    if (callbackSetRun)
    {
#ifdef __DBG_J0001__
      Serial.printf("callbackSetRun=%d\n", runState);
#endif
      callbackSetRun(this, runState);
    }
  }
  else if (functionCode == 0x11)
  {
    float dropletVolume = ProtocolStd::popFloat(frame);
#ifdef __DBG_J0001__
    Serial.printf("1 dropletVolume=%f\n", dropletVolume);
#endif
    if (callbackSetDropletVolume)
    {
      callbackSetDropletVolume(this, dropletVolume);
    }
  }
  else if (functionCode == 0x12)
  {
    uint8_t windowDuration = ProtocolStd::popUint8_t(frame);
    if (callbackSetWindowDuration)
    {
      callbackSetWindowDuration(this, windowDuration);
    }
  }
  else if(functionCode == 0x13)
  {
      callbackResetMeasurement(this);
  }
  else if (functionCode == 0x14)
  {
    float maxFlowDisplay = ProtocolStd::popFloat(frame);
    if (callbackSetMaxFlowDisplay)
    {
      callbackSetMaxFlowDisplay(this, maxFlowDisplay);
    }
  }
}
