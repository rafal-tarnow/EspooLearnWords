#pragma once
#include <Arduino.h>
#include <Ethernet.h>
#include <cstdint>
#include <set>
#include <lwip/tcp.h>
#include <TcpSocket.hpp>

class TcpServer
{
    using CallbackFunction = std::function<void()>;

public:
    ~TcpServer();
    void onNewConnection(CallbackFunction callback)
    {
        newConnectionCallbackFunction = callback;
    }
    template <typename T>
    void onNewConnection(T *obj, void (T::*method)())
    {
        newConnectionCallbackMethod = [obj, method]()
        {
            (obj->*method)();
        };
    }

    bool listen(uint16_t port);
    void close();
    bool hasPendingConnections();
    TcpSocket *nextPendingConnection();

private:
    static err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);

private:
    CallbackFunction newConnectionCallbackFunction;
    CallbackFunction newConnectionCallbackMethod;
    std::set<TcpSocket *> pendingConnections;
    //std::set<TcpSocket *> allConnections;
    struct tcp_pcb *server_pcb = NULL;
};