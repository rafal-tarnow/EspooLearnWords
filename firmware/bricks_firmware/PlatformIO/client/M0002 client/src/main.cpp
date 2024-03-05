#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#include <OneButton.h>
#include "led.h"
#include "ApplicationK0002.h"
#include "ApplicationM0004.hpp"
#include "ApplicationT0002.h"
#include "T0002.hpp"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "config.h"
#include "Debug.hpp"

#define BUTTON_PIN 0
#define LED_PIN 2

OneButton button = OneButton(
    BUTTON_PIN, // Input pin for the button
    true,       // Button is active LOW
    false       // Enable internal pull-up resistor
);

#ifdef __BRICK_K0002__
ApplicationM0002 application;
#elif defined(__BRICK_M0004__)
ApplicationM0004 application;
#elif defined(__BRICK_T0002__)
ApplicationT0002 application;
#endif


void setupOTA();
const char* reasonToCstr(rst_reason reason);
void printResetReason();

void handleLongPress()
{
  application.handleButtonLongPress();
}

void handleClick()
{
  application.handleButtonClick();
}

void handleDoubleClick()
{
  application.handleButtonDoubleClick();
}

void setup()
{
  // Setup serial
  // Serial.begin(9600);
  //Serial.begin(115200);
  delay(100);
  Serial.begin(3000000);

  printResetReason();
  //delay(12000);


  // Setup led
  LedInit(LED_PIN);
  LedSetState(false);

  button.attachLongPressStart(handleLongPress);
  button.attachClick(handleClick);
  button.attachDoubleClick(handleDoubleClick);
  button.setPressMs(7000);

  application.begin();

  setupOTA();
}

void loop()
{
  Serial.printf("\n%lu --->>> main::loop()", dtime());



     application.update();


  button.tick();
  LedTick();
  //ArduinoOTA.handle();
}

void setupOTA()
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });

  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });

  ArduinoOTA.begin();
}



void printResetReason(){
    // Odczytaj informacje o resecie
  struct rst_info *resetInfo = ESP.getResetInfoPtr();

  // Wyświetl informacje na Serial Monitor
  Serial.println("\n Powód resetu:");
  Serial.printf("   Przyczyna: %s\n", reasonToCstr(rst_reason(resetInfo->reason)));
  Serial.printf("   Adres błędu: 0x%08X\n", resetInfo->exccause);
  Serial.printf("   Adres epc1: 0x%08X\n", resetInfo->epc1);
  Serial.printf("   Adres epc2: 0x%08X\n", resetInfo->epc2);
  Serial.printf("   Adres epc3: 0x%08X\n", resetInfo->epc3);
  Serial.printf("   Adres excvaddr: 0x%08X\n", resetInfo->excvaddr);
  Serial.printf("   Adres depc: 0x%08X\n", resetInfo->depc);
}

const char* reasonToCstr(rst_reason reason) {
    switch (reason) {
        case REASON_DEFAULT_RST:
            return "Normal startup by power on";
        case REASON_WDT_RST:
            return "Hardware watchdog reset";
        case REASON_EXCEPTION_RST:
            return "Exception reset, GPIO status won't change";
        case REASON_SOFT_WDT_RST:
            return "Software watchdog reset, GPIO status won't change";
        case REASON_SOFT_RESTART:
            return "Software restart, system_restart, GPIO status won't change";
        case REASON_DEEP_SLEEP_AWAKE:
            return "Wake up from deep-sleep";
        case REASON_EXT_SYS_RST:
            return "External system reset";
        default:
            return "Unknown reset reason";
    }
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
/*
#include <ESP8266WiFi.h>
#include <lwip/tcp.h>
#include <PseudoDNS.h>
#include <set>
#include <map>
#include <deque>
#include <utility>
#include "ProtocolStd.h"
#include "dtime.h"

// Parametry serwera TCP
const int server_port = 2883;
uint32_t newConnection = 0;
uint32_t deleteConnection = 0;

std::map<struct tcp_pcb *, ProtocolStd *> pcbToProtocol;

struct tcp_pcb *findPcbByProtocol(ProtocolStd *protocol)
{
  for (const auto &pair : pcbToProtocol)
  {
    if (pair.second == protocol)
    {
      return pair.first; // Znaleziono pasujący wpis, zwraca wskaźnik do tcp_pcb
    }
  }
  return nullptr; // Brak pasującego wpisu, zwraca nullptr
}

void sendProtocolFrame(ProtocolStd *proto, std::vector<uint8_t> &frame)
{
  //Serial.printf("\n%lu sendProtocolFrame() ", dtime());
  uint16_t size = frame.size();
  std::vector<uint8_t> datagram;
  ProtocolStd::append(datagram, size);
  datagram.insert(datagram.end(), frame.begin(), frame.end());
  struct tcp_pcb *pcb = findPcbByProtocol(proto);

  if (pcb != NULL && pcb->state == ESTABLISHED)
  {
    // Użyj tcp_write do wysłania danych
    //Serial.printf("\n%lu write to tcp ", dtime());
    err_t result = tcp_write(pcb, datagram.data(), datagram.size(), TCP_WRITE_FLAG_COPY);
  }
}

void procotolCallback(ProtocolStd *proto, std::deque<uint8_t> &frame)
{
  //Serial.printf("procotolCallback()");

  if (!frame.empty())
  {
    uint8_t functionCode = ProtocolStd::getUint8_t(frame);
    //Serial.printf("\n%lu functionCode == %d", dtime(), functionCode);
    if (functionCode == 0x01) // ping command
    {
      // Serial.println("\nfunctionCode == 0x01");
      std::vector<uint8_t> frame;
      ProtocolStd::append(frame, uint8_t(0x01));
      sendProtocolFrame(proto, frame);
    }
    else if (functionCode == 0x02)
    {
      std::vector<uint8_t> frame;
      ProtocolStd::append(frame, uint8_t(0x02));
      ProtocolStd::append(frame, "T0002");
      ProtocolStd::append(frame, "Elo czujnik");
      sendProtocolFrame(proto, frame);
    }
  }
}

// Callback funkcja dla zdarzenia odebrania danych
err_t receive_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  if (p != NULL)
  {
    //Serial.println("\nOdebrano dane:");

    while (p != NULL)
    {
      // Tutaj możesz przetwarzać i wyświetlać odebrane dane
      // Serial.write((const char *)p->payload, p->len);
      std::vector<uint8_t> byteVector((const char *)p->payload, (const char *)p->payload + p->len);
      pcbToProtocol[tpcb]->addData(byteVector);
      //       std::vector<uint8_t> frame;
      // ProtocolStd::append(frame, uint8_t(0x02));
      // ProtocolStd::append(frame, "T0002");
      // ProtocolStd::append(frame, "Elo czujnik");
      // sendProtocolFrame(pcbToProtocol[tpcb], frame);
      tcp_recved(tpcb, p->len);


      struct pbuf *next_p = p->next; // Zapamiętaj wskaźnik do następnego bufora
      pbuf_free(p);                  // Zwolnij aktualny bufor
      p = next_p;                    // Przejdź do następnego bufora
    }
  }
  else
  {
    // Połączenie zostało zamknięte lub wystąpił inny błąd
    //Serial.println("\nPołączenie zamknięte lub błąd.");
    // Możesz obsłużyć zamknięcie połączenia tutaj
    delete pcbToProtocol[tpcb];
    pcbToProtocol.erase(tpcb);
    // Zamknij połączenie
    deleteConnection++;
    tcp_close(tpcb);
  }
  return ERR_OK;
}

// Callback funkcja dla zdarzenia zamknięcia połączenia
void error_callback(void *arg, err_t err)
{
  // Tutaj możesz obsłużyć zamknięcie połączenia
  Serial.println("Error callback");
}

err_t sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  //Serial.printf("\nDostarczono pakiet");
  return ERR_OK;
}

// Callback funkcja dla zdarzenia połączenia
err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  if (err == ERR_OK)
  {
    //Serial.println("Połączenie nawiązane.");

    // Wypisz adres IP klienta
    //Serial.print("Klient połączył się z adresem IP: ");
    //Serial.println(IPAddress(newpcb->remote_ip).toString());

    // Ustawienie callback funkcji dla zdarzenia odebrania danych

    tcp_err(newpcb, error_callback);
    tcp_sent(newpcb, sent_callback);
    tcp_recv(newpcb, receive_callback);
    newConnection++;
    ProtocolStd *protocol = new ProtocolStd();
    protocol->setOnFrameCallback(procotolCallback);
    pcbToProtocol[newpcb] = protocol;
  }
  else
  {
    Serial.printf("Błąd połączenia: %d\n", err);
  }
  return err;
}

PseudoDNS dns;

struct tcp_pcb *server_pcb = NULL;

void openTcpServer()
{
  // Utworzenie serwera TCP
  server_pcb = tcp_new();
  if (server_pcb != NULL)
  {
    // Ustawienie callback funkcji dla zdarzenia połączenia
    tcp_bind(server_pcb, IP_ADDR_ANY, server_port);
    server_pcb = tcp_listen(server_pcb);

    tcp_accept(server_pcb, accept_callback);
    Serial.printf("Serwer nasłuchuje na porcie %d\n", server_port);
  }
  else
  {
    Serial.println("Nie można utworzyć serwera TCP.");
  }
}

void closeTcpServer()
{
  if (server_pcb != NULL)
  {
    // Zamknij serwer i zwolnij zasoby
    tcp_close(server_pcb);
    server_pcb = NULL;
    Serial.println("Serwer TCP został zamknięty.");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Inicjalizacja WiFi
  WiFi.begin("TP-LINK_A1AE89", "Krzysiu1");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Łączenie z WiFi...");
  }
  Serial.println("Połączono z WiFi.");

  // Wypisz adres IP urządzenia
  Serial.print("Adres IP urządzenia: ");
  Serial.println(WiFi.localIP());

  dns.run("Elo czujnik");
  openTcpServer();
}

void loop()
{
  // Możesz dodać tutaj inne czynności, jeśli są potrzebne
  // Pobranie ilości wolnej pamięci
  size_t freeHeap = ESP.getFreeHeap();

  dns.update();

  unsigned long current = millis();
  static unsigned long previous = 0;
  if (current - previous > 3000)
  {
    previous = current;
    //Serial.printf("\nCo sekunde");
    ObjectCounter::printArduino();
    Serial.printf("\nWolna pamięć RAM: %d", freeHeap);
    Serial.printf("\nnewConnection: %d", newConnection);
    Serial.printf("\ndeleteConnection: %d", deleteConnection);
  }

}
*/