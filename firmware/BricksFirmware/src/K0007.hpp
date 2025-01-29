#pragma once
#include "BrickClient.hpp"
#include "Debug.hpp"

//single switch 

class K0007 : public BrickClient
{
public:
    K0007(AsyncClient *asyncClient);
    K0007(TcpSocket *tcpSocket);

    void cmdSetSwitchState(bool switchState);
    static std::string type();
    void processProtocolStdFrame(std::deque<uint8_t> &frame) override;

private:
    DBG_COUNT("K0007");
};