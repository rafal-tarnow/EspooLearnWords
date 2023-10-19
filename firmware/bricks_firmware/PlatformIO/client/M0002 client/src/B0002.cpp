#include "B0002.hpp"

 B0002::B0002(AsyncClient *asyncClient) : BrickClient(asyncClient) {  
}

B0002::B0002(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void B0002::cmdSetMeasureTemp(float temp)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, temp);
  sendProtocolFrame(frame);
}

std::string B0002::type()
{
    return "B0002";
}
