#include "AspooServer.h"

std::vector<AspooClient *> AspooServer::mClients;

void AspooServer::addClient(AspooClient *client){
  mClients.push_back(client);
}

void AspooServer::setName(std::string name) {
  mName = name;
}

bool AspooServer::checkAllClientsConnected(){
    for (AspooClient* client : mClients) {
        if (!client->isConnected()) {
            return false;
        }
    }
    return true;
}

void AspooServer::loop() {

  if(!checkAllClientsConnected()){
    if(!pseudoDNS.isRunning())
      pseudoDNS.startServer(mName);
  }else{
    if(pseudoDNS.isRunning())
      pseudoDNS.stopServer();
  }

  pseudoDNS.loop();
}