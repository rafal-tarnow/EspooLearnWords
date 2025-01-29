#include "T0002.hpp"

T0002::T0002(AsyncClient *asyncClient) : BrickClient(asyncClient)
{
#ifdef __DBG_T0002__
  Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
}

T0002::T0002(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
#ifdef __DBG_T0002__
  Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
}

void T0002::cmdSetMeasureTempPressHum(float temp, float pressure, float humidity)
{
#ifdef __DBG_T0002__
  Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, temp);
  ProtocolStd::append(frame, pressure);
  ProtocolStd::append(frame, humidity);
  sendProtocolFrame(frame);
}

void T0002::cmdSetTestValue(uint64_t index, std::string text)
{
#ifdef __DBG_T0002__
  Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x11));
  ProtocolStd::append(frame, index);
  ProtocolStd::append(frame, text);
  sendProtocolFrame(frame);
}

void T0002::cmdSetFrequency(float freq)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x12));
  ProtocolStd::append(frame, freq);
  sendProtocolFrame(frame);
}

std::string T0002::type()
{
#ifdef __DBG_T0002__
  Serial.printf("%lu %s\n", dtime(), __PRETTY_FUNCTION__);
#endif
  return "T0002";
}

void T0002::processProtocolStdFrame(std::deque<uint8_t> &frame)
{
  uint8_t functionCode = ProtocolStd::popUint8_t(frame);
  if(functionCode == 0x11)
  {
    float frequency = ProtocolStd::popFloat(frame);
    if(callbackSetFrequency)
    {
      callbackSetFrequency(this, frequency);
    }
  }
}
