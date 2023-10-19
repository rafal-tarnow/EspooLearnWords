#include "TcpServer.hpp"

TcpServer::~TcpServer()
{

    // for (auto it = allConnections.begin(); it != allConnections.end(); ++it)
    // {
    //     TcpSocket *socket = *it;
    //     socket->close();
    //     delete socket;
    //     socket = nullptr;
    // }
    // allConnections.clear();

    close();
}

void TcpServer::close()
{
    if (server_pcb != NULL)
    {
        tcp_close(server_pcb);
        Serial.println("Serwer TCP został zamknięty.");
    }
}

// Callback funkcja dla zdarzenia połączenia
err_t TcpServer::accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    Serial.printf("\n TcpServer::accept_callback()");
    TcpServer *tcpServer = reinterpret_cast<TcpServer *>(arg);

    if (err == ERR_OK)
    {
        Serial.printf("\nPołączenie nawiązane.");

        // Wypisz adres IP klienta
        Serial.printf("\nKlient połączył się z adresem IP: ");
        Serial.print(IPAddress(newpcb->remote_ip).toString());
        TcpSocket *tcpSocket = new TcpSocket(newpcb);
        //tcpServer->allConnections.insert(tcpSocket);
        tcpServer->pendingConnections.insert(tcpSocket);

        if (tcpServer->newConnectionCallbackFunction)
        {
            tcpServer->newConnectionCallbackFunction();
        }
        if (tcpServer->newConnectionCallbackMethod)
        {
            tcpServer->newConnectionCallbackMethod();
        }
        // Ustawienie callback funkcji dla zdarzenia odebrania danych
    }
    else
    {
        Serial.printf("Błąd połączenia: %d\n", err);
    }
    return err;
}

bool TcpServer::listen(uint16_t server_port)
{

    server_pcb = tcp_new();
    if (server_pcb != NULL)
    {
        // Ustawienie callback funkcji dla zdarzenia połączenia
        tcp_bind(server_pcb, IP_ADDR_ANY, server_port);
        server_pcb = tcp_listen(server_pcb);
        server_pcb->callback_arg = this;
        tcp_accept(server_pcb, TcpServer::accept_callback);
        Serial.printf("Serwer nasłuchuje na porcie %d\n", server_port);
        return true;
    }
    else
    {
        Serial.println("Nie można utworzyć serwera TCP.");
        return false;
    }
    return false;
}

bool TcpServer::hasPendingConnections()
{
    return pendingConnections.size() > 0;
}

TcpSocket *TcpServer::nextPendingConnection()
{
    if (!pendingConnections.empty())
    {
        auto it = pendingConnections.begin();
        TcpSocket *nextConnection = *it;
        pendingConnections.erase(it);
        return nextConnection;
    }
    else
    {
        return nullptr;
    }
}
