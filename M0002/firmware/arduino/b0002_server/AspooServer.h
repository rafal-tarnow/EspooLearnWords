#pragma once

#include <string>
#include <ESP8266mDNS.h>

#include "AspooClient.h"
#include "PseudoDNS.h"
#include "B0002.h"

class AspooServer{
  friend class AspooClient;
public:
  void begin(std::string name);
  void update();
private:
  static void addClient(AspooClient * client);
  bool checkAllClientsConnected();
private:
  static std::vector<AspooClient *> mClients;
  std::string mName;
  bool mAllClientsConnected = false;
  PseudoDNS pseudoDNS;
};