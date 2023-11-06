#include "PseudoDNS.h"

void PseudoDNS::setHostName(const std::string &hostName)
{
  mName = hostName;
}

void PseudoDNS::run(const std::string &brickId, const std::string &brickType, const std::string &brickName)
{
  Serial.printf("\n PseudoDNS::run() hostName = %s", brickName.c_str());
  mRunning = true;
  mId = brickId;
  mType = brickType;
  mName = brickName;
  Udp.begin(PORT);
}

bool PseudoDNS::isRunning()
{
  Serial.println("\n PseudoDNS::isRunning()");
  return mRunning;
}

void PseudoDNS::stopRunning()
{
  Serial.println("\n PseudoDNS::stopRunning()");
  mRunning = false;
  if (runQuery == false)
  {
    Udp.stop();
  }
}

void PseudoDNS::startQueriesForAllHosts()
{
  Serial.println("\n PseudoDNS::startQueriesForAllHosts()");
  runQuery = true;
  Udp.begin(PORT);
  onQueryTime(); // send first query immidietly
}

bool PseudoDNS::isQueriesRunning()
{
  Serial.println("\n PseudoDNS::isQueriesRunning()");
  return runQuery;
}

void PseudoDNS::stopQueries()
{
  Serial.println("\n PseudoDNS::stopQueries()");
  runQuery = false;
  dnsDiscoverdHosts.clear();
  if (mRunning == false)
  {
    Udp.stop();
  }
}

IPAddress PseudoDNS::softAPbroadcastIP()
{
  struct ip_info ip;
  wifi_get_ip_info(SOFTAP_IF, &ip);
  return IPAddress(ip.ip.addr | ~(ip.netmask.addr));
}

std::string PseudoDNS::getIPById(std::string id)
{
  return dnsDiscoverdHosts[id].hostIp;
}

bool PseudoDNS::hasIP(std::string id)
{
  return (dnsDiscoverdHosts.find(id) != dnsDiscoverdHosts.end());
}

void PseudoDNS::onQueryTime()
{
  // Serial.println("\n PseudoDNS::onQueryTime()");
  IPAddress broadcast;
  if (WIFI_AP == WiFi.getMode())
  {
    broadcast = softAPbroadcastIP();
  }
  else
  {
    broadcast = WiFi.broadcastIP();
  }
  Udp.beginPacket(broadcast, PORT);
  Udp.write(0x01);
  Udp.endPacket();
  // Serial.println("\n--->Send Query DNS package");
}

void PseudoDNS::onUdpDatagram(int packetSize)
{
// Serial.println("\n PseudoDNS::onUdpDatagram()");
#warning "TODO data reading from the buffer can be optimized"
  byte buffer[packetSize];
  int len = Udp.read(buffer, sizeof(buffer));

  std::deque<uint8_t> dataDeque(buffer, buffer + len);
  // std::vector<uint8_t> receivedData;
  // int len = Udp.read(receivedData.data(), packetSize);

  if (len > 0)
  {
    // buffer[len] = 0;
    //  Serial.println("<----Received packet from: " + Udp.remoteIP().toString() + " port: " + String(Udp.remotePort()) + " size: " + String(len) + " with data: " + String((char*)buffer));
    uint8_t functionCode = ProtocolStd::getUint8_t(dataDeque);
    if (functionCode == 0x01)
    // if (receivedData[0] == 0x01)
    {
      if (mRunning)
      {
        std::vector<uint8_t> frame;
        ProtocolStd::append(frame, uint8_t(0x02));
        ProtocolStd::append(frame, mId);
        ProtocolStd::append(frame, mType);
        ProtocolStd::append(frame, mName);
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(frame.data(), frame.size());
        Udp.endPacket();
        // Serial.println("\n--->Send response package");
      }
    }
    else if (functionCode == 0x02)
    {
      //Serial.println("0x02");
      if (runQuery)
      {
        HostInfo hostInfo;

        hostInfo.hostId = ProtocolStd::getString(dataDeque);
        hostInfo.hostType = ProtocolStd::getString(dataDeque);
        hostInfo.hostName = ProtocolStd::getString(dataDeque);
        hostInfo.hostIp = Udp.remoteIP().toString().c_str();

        // Serial.println("0x02");
        // Serial.println(hostInfo.hostId.c_str());
        // Serial.println(hostInfo.hostType.c_str());
        // Serial.println(hostInfo.hostName.c_str());
        // Serial.println(hostInfo.hostIp.c_str());

        auto iter = dnsDiscoverdHosts.find(hostInfo.hostId);

        if (iter == dnsDiscoverdHosts.end() || (iter->second != hostInfo))
        {
          dnsDiscoverdHosts[hostInfo.hostId] = hostInfo;

          for (auto listener : eventListeners)
          {
            listener->handleOnHostFound(hostInfo.hostId, hostInfo.hostType, hostInfo.hostName, hostInfo.hostIp);
          }

          if (callbackFunction)
          {
            callbackFunction(hostInfo.hostId, hostInfo.hostType, hostInfo.hostName, hostInfo.hostIp);
          }
          if (callbackMethod)
          {
            callbackFunction(hostInfo.hostId, hostInfo.hostType, hostInfo.hostName, hostInfo.hostIp);
          }
        }
      }
    }
  }
}

void PseudoDNS::update()
{
  // Serial.println("\n PseudoDNS::update()");
  if (mRunning || runQuery)
  {
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      onUdpDatagram(packetSize);
    }
  }

  //
  if (runQuery)
  {
    // Serial.print("PseudoDNS::update() 1");
    unsigned long currentTime = millis();
    if (currentTime - lastQueryTime >= QUERY_INTERVAL)
    {
      onQueryTime();
      lastQueryTime = currentTime;
    }
  }
}