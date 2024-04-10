#pragma once
#include "BrickClient.hpp"
#include "Debug.hpp"

class K0002Client : public BrickClient
{
public:
    K0002Client(AsyncClient *asyncClient);
    K0002Client(TcpSocket *tcpSocket);
    void cmdSetMeasureTemp(float temp);
    static std::string type();
    void processProtocolStdFrame(std::deque<uint8_t> &frame) override;


private:
    DBG_COUNT("K0002Client");
};