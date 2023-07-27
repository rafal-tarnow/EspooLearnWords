#include "AspooServer.h"

std::vector<AspooClient *> AspooServer::mClients;

void AspooServer::addClient(AspooClient *client) {
  mClients.push_back(client);
}

bool AspooServer::checkAllClientsConnected() {
  for (AspooClient *client : mClients) {
    if (!client->isConnected()) {
      return false;
    }
  }
  return true;
}

 void AspooServer::foundHost(const std::string & hostName, const std::string & hostIP){
   Serial.print("AspooServer::foundHost: ");
   Serial.print(hostName.c_str());
   Serial.print(" ");
   Serial.println(hostIP.c_str());
 }

void AspooServer::begin(std::string name) {
  mName = name;
  pseudoDNS.run(mName);
  pseudoDNS.onHostFound(this, &AspooServer::foundHost);
  pseudoDNS.startQueriesForAllHosts();
    // Start mDNS
  // if (!MDNS.begin(mName.c_str())) {
  //   Serial.println("Error setting up mDNS!");
  //   while (1) {
  //     delay(1000);
  //   }
  // }
}

void AspooServer::update() {


  //pseudoDNS
  // if (!checkAllClientsConnected()) {
  //   if (!pseudoDNS.isRunning())
  //     pseudoDNS.startServer(mName);
  // } else {
  //   if (pseudoDNS.isRunning())
  //     pseudoDNS.stopServer();
  // }

  pseudoDNS.update();

  //mDNS
  //  if (!checkAllClientsConnected()){
     
  //  }
}