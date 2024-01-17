#include "M0004.hpp"

 M0004::M0004(AsyncClient *asyncClient) : BrickClient(asyncClient) {  
}

M0004::M0004(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void M0004::cmdSetMeasureTemp(float temp)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, temp);
  sendProtocolFrame(frame);
}

std::string M0004::type()
{
    return "M0004";
}
