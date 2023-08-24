#include "B0002.hpp"

void B0002::cmdSetMeasureTemp(AsyncClient * client, float temp)
{
  std::vector<uint8_t> frame;
  ProtocolStd::append(frame, uint8_t(0x10));
  ProtocolStd::append(frame, temp);
  sendProtocolFrame(client, frame);
}

std::string B0002::getBrickType() const
{
  return "B0002";
}