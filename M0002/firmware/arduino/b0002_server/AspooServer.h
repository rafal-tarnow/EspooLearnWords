#pragma once

#include <string>

#include "AspooClient.h"
#include "PseudoDNS.h"
#include "B0002.h"

class AspooServer{
  friend class AspooClient;
public:
  void setName(std::string name);
  void loop();
private:
  static void addClient(AspooClient * client);
  bool checkAllClientsConnected();
private:
  static std::vector<AspooClient *> mClients;
  std::string mName;
  bool mAllClientsConnected = false;
  PseudoDNS pseudoDNS;
};