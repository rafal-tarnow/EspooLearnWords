#include "T0002.hpp"

T0002::T0002(AsyncClient *asyncClient) : BrickClient(asyncClient)
{
}

T0002::T0002(TcpSocket *tcpSocket) : BrickClient(tcpSocket)
{
}

void T0002::cmdSetMeasureTempHumPress(float temp, float humidity, float pressure)
{
    Serial.printf("\n%lu T0002::cmdSetMeasureTempHumPress()", dtime());
    std::vector<uint8_t> frame;
    ProtocolStd::append(frame, uint8_t(0x10));
    ProtocolStd::append(frame, temp);
    ProtocolStd::append(frame, humidity);
    ProtocolStd::append(frame, pressure);
    sendProtocolFrame(frame);
}

void T0002::cmdSetTestValue(uint64_t index, std::string text)
{
    Serial.printf("\n%lu T0002::cmdSetTestValue()", dtime());
    std::vector<uint8_t> frame;
    ProtocolStd::append(frame, uint8_t(0x11));
    ProtocolStd::append(frame, index);
    ProtocolStd::append(frame, text);

  for (const uint8_t &value : frame) {
    Serial.print(value);
    Serial.print(" ");
  }
  Serial.println(); // Nowa linia na końcu

    sendProtocolFrame(frame);
}

std::string T0002::type()
{
    return "T0002";
}
