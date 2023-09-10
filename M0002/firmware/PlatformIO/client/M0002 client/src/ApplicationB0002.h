#pragma once
#include <set>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Application.h"
#include "B0002.hpp"

// class ApplicationB0002 : public Application
// {
// public:
//   ApplicationB0002();
//   void begin();
//   void update() override;
//   std::string getBrickType() const override;
//    void handleNewTcpClient(void *arg, AsyncClient *client) override;
//   void handleNewTcpSocket(TcpSocket * socket) override;

// private:
//   std::set<B0002 *> clients;
//   const int oneWirePin = 4;
//   OneWire oneWire;
//   DallasTemperature sensors;
// };