#pragma once

#include <string>

class AspooClient{
public:
  AspooClient();
  void setName(std::string name);
  bool isConnected();
private:
  std::string mName;
  bool mIsConnected = false;
};