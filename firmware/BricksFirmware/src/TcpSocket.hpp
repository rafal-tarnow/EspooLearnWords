#pragma once
#include <lwip/tcp.h>
#include <Arduino.h>
#include <functional>
#include <string>
#include "Debug.hpp"

class TcpSocket
{

public:
    using DataCallback = std::function<void(const uint8_t *, uint16_t len)>;
    using DisconnectCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const std::string&)>;

    TcpSocket(struct tcp_pcb *newpcb);
    template <typename T>
    void onData(T *obj, void (T::*method)(const uint8_t *buffer, uint16_t len))
    {
        dataCallback = [obj, method](const uint8_t *buffer, uint16_t len)
        {
            (obj->*method)(buffer, len);
        };
        socket_pcb->callback_arg = this;
        tcp_recv(socket_pcb, receive_callback);
    }
    template <typename T>
    void onDisconnected(T *obj, void (T::*method)())
    {
        disconnectCallback = [obj, method]()
        {
            (obj->*method)();
        };
    }
    template <typename T>
    void onError(T *obj, void(T::*method)(const std::string & error))
    {
        errorCallback = [obj, method](const std::string & error)
        {
            (obj->*method)(error);
        };
    }
    uint16_t write(const uint8_t *buff, uint16_t len);
    void close();
    void abort();

private:
    static err_t receive_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
    static err_t sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len);
    static void error_callback(void *arg, err_t err);

    static const char *err_to_cstr(err_t err);

private:
    DataCallback dataCallback;
    DisconnectCallback disconnectCallback;
    ErrorCallback errorCallback;
    struct tcp_pcb *socket_pcb = NULL;
     DBG_COUNT("TcpSocket");
};