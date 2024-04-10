#include "K0002Client.hpp"

K0002Client::K0002Client(AsyncClient *asyncClient) : BrickClient(asyncClient) {  
}

K0002Client::K0002Client(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void K0002Client::cmdSetMeasureTemp(float temp)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, temp);
  sendProtocolFrame(frame);
}

std::string K0002Client::type()
{
    return "K0002";
}

void K0002Client::processProtocolStdFrame(std::deque<uint8_t> &frame)
{
}
