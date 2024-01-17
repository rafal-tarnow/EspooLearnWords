#pragma once
#include "BrickClient.hpp"
#include "Debug.hpp"

class M0002 : public BrickClient
{
public:
    M0002(AsyncClient *asyncClient);
    M0002(TcpSocket *tcpSocket);
    void cmdSetMeasureTemp(float temp);
    static std::string type();

private:
    DBG_COUNT("M0002");
};