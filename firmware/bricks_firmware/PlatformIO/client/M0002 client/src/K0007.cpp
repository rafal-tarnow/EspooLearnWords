#include "K0007.hpp"

K0007::K0007(AsyncClient *asyncClient) : BrickClient(asyncClient)
{
}

K0007::K0007(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void K0007::cmdSetSwitchState(bool switchState)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, switchState);
  sendProtocolFrame(frame);
}

std::string K0007::type()
{
  return "K0007";
}

void K0007::processProtocolStdFrame(std::deque<uint8_t> &frame)
{
#ifdef __DBG_K0007__
  Serial.printf("%s\n", __PRETTY_FUNCTION__);
#endif
}
