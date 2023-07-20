#include "AspooClient.h"
#include "AspooServer.h"

AspooClient::AspooClient() {
  AspooServer::addClient(this);
}

void AspooClient::setName(std::string name) {
  mName = name;
}

bool AspooClient::isConnected(){
  return mIsConnected;
}