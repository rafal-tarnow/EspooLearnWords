#include "M0004.hpp"

M0004::M0004(AsyncClient *asyncClient) : BrickClient(asyncClient)
{
}

M0004::M0004(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

std::string M0004::type()
{
  return "K0004";
}

void M0004::processProtocolStdFrame(std::deque<uint8_t> &frame)
{
#ifdef __DBG_K0004__
  Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
  uint8_t functionCode = ProtocolStd::popUint8_t(frame);
  if (functionCode == 0x10)
  {
    bool relayState = ProtocolStd::popBool(frame);
#ifdef __DBG_K0004__
    Serial.printf("relayState=%d\n", relayState);
#endif
    if (callbackSetRelay)
    {
#ifdef __DBG_K0004__
      Serial.printf("callbackSetRelay=%d\n", relayState);
#endif
      callbackSetRelay(this, relayState);
    }
  }
}
