#include "M0002.hpp"

 M0002::M0002(AsyncClient *asyncClient) : BrickClient(asyncClient) {  
}

M0002::M0002(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void M0002::cmdSetMeasureTemp(float temp)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, temp);
  sendProtocolFrame(frame);
}

std::string M0002::type()
{
    return "M0002";
}
