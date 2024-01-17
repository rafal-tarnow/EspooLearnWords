#pragma once
#include "BrickClient.hpp"
#include "Debug.hpp"

class M0004 : public BrickClient
{
public:
    M0004(AsyncClient *asyncClient);
    M0004(TcpSocket *tcpSocket);
    void cmdSetMeasureTemp(float temp);
    static std::string type();

private:
    DBG_COUNT("M0004");
};