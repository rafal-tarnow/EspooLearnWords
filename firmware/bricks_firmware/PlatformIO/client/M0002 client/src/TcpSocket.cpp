#include "TcpSocket.hpp"
#include "dtime.h"

TcpSocket::TcpSocket(tcp_pcb *newpcb)
{
    if (newpcb != NULL && newpcb->state == ESTABLISHED)
    {
        socket_pcb = newpcb;

        tcp_arg(socket_pcb, this);
        tcp_err(socket_pcb, error_callback);
        tcp_arg(socket_pcb, this);
        tcp_sent(socket_pcb, sent_callback);
    }
    else
    {
        Serial.printf("%lu TcpSocket::TcpSocket() Error", dtime());
        ESP.reset();
    }
}

uint16_t TcpSocket::write(const uint8_t *buff, uint16_t len)
{
    if (socket_pcb != NULL && socket_pcb->state == ESTABLISHED)
    {
        // Użyj tcp_write do wysłania danych
        err_t result = tcp_write(socket_pcb, buff, len, TCP_WRITE_FLAG_COPY);

        if (result == ERR_OK)
        {
            return len;
        }
        else if (result == ERR_MEM)
        {
            // Brak pamięci na buforze, spróbuj ponownie później
            Serial.printf("%lu TcpSocket memory error", dtime());
            ESP.reset();
        }
        else
        {
            // Błąd podczas wysyłania danych, możesz to obsłużyć
            Serial.printf("%lu Error while sending socket data", dtime());
            ESP.reset();
        }
    }
    else
    {
        // Socket jest zamknięty, nie można wysyłać danych
        Serial.printf("%lu Socket closed while sendinga data", dtime());
        ESP.reset();
    }
    return 0;
}

void TcpSocket::close()
{
    if (socket_pcb != NULL)
    {
        tcp_close(socket_pcb);
        socket_pcb = NULL;
    }
}

void TcpSocket::abort()
{
    if (socket_pcb != NULL)
    {
        tcp_abort(socket_pcb);
        socket_pcb = NULL;
    }
}

err_t TcpSocket::receive_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    Serial.printf("\n%lu TcpSocket::receive_callback()", dtime());
    TcpSocket *tcpSocket = reinterpret_cast<TcpSocket *>(arg);
    if (p != NULL)
    {
        while (p != NULL)
        {
            if (tcpSocket->dataCallback)
            {
                tcpSocket->dataCallback((const uint8_t *)p->payload, p->len);
            }
            tcp_recved(tpcb, p->len);

            struct pbuf *next_p = p->next; 
            pbuf_free(p);
            p = next_p;
        }
    }
    else
    {
        tcpSocket->close();
        if (tcpSocket->disconnectCallback)
        {
            tcpSocket->disconnectCallback();
        }
        return ERR_OK;
    }
    return ERR_OK;
}

err_t TcpSocket::sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    // Serial.printf("\n%lu TcpSocket::sent_callback()", dtime());
    return ERR_OK;
}

void TcpSocket::error_callback(void *arg, err_t err)
{
    //The corresponding pcb is already freed when this callback is called!
    Serial.printf("\n%lu TcpSocket::error_callback()", dtime());
    TcpSocket *tcpSocket = reinterpret_cast<TcpSocket *>(arg);
    Serial.printf("\n%lu 1", dtime());
    if(tcpSocket->errorCallback){
        Serial.printf("\n%lu 2", dtime());
        tcpSocket->errorCallback(std::string(err_to_cstr(err)));
        Serial.printf("\n%lu 3", dtime());
        ESP.reset();
    }
}

const char *TcpSocket::err_to_cstr(err_t err)
{
    switch (err)
    {
    case ERR_OK:
        return "No error, everything OK";
    case ERR_MEM:
        return "Out of memory error";
    case ERR_BUF:
        return "Buffer error";
    case ERR_TIMEOUT:
        return "Timeout";
    case ERR_RTE:
        return "Routing problem";
    case ERR_INPROGRESS:
        return "Operation in progress";
    case ERR_VAL:
        return "Illegal value";
    case ERR_WOULDBLOCK:
        return "Operation would block";
    case ERR_USE:
        return "Address in use";
    case ERR_ALREADY:
        return "Already connecting";
    case ERR_ISCONN:
        return "Connection already established";
    case ERR_CONN:
        return "Not connected";
    case ERR_IF:
        return "Low-level netif error";
    case ERR_ABRT:
        return "Connection aborted";
    case ERR_RST:
        return "Connection reset";
    case ERR_CLSD:
        return "Connection closed";
    case ERR_ARG:
        return "Illegal argument";
    default:
        return "Unknown error";
    }
}
