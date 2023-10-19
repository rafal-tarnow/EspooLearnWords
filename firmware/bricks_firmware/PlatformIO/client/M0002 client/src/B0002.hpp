#pragma once
#include "BrickClient.hpp"
#include "Debug.hpp"

class B0002 : public BrickClient
{
public:
    B0002(AsyncClient *asyncClient);
    B0002(TcpSocket *tcpSocket);
    void cmdSetMeasureTemp(float temp);
    static std::string type();

private:
    DBG_COUNT("T0002");
};