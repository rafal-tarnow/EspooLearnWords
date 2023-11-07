#include "AspooBricks.h"

PseudoDNS KiKoBrick::pseudoDNS;
std::vector<KiKoBrick *> KiKoBrick::bricks;

KiKoBrick::KiKoBrick()
{
    Serial.println("Create new AspooBrick object");
}

void KiKoBrick::handleOnHostFound(const std::string &id, const std::string &type, const std::string &name, const std::string &ip)
{
    Serial.println("KiKoBrick:: handle DNS data:");
    Serial.printf("     %s\n", id.c_str());
    Serial.printf("     %s\n", type.c_str());
    Serial.printf("     %s\n", name.c_str());
    Serial.printf("     %s\n", ip.c_str());
    if (stateMachine.getCuttentState() == BrickState::INIT)
    {
        Serial.println("Brick is in INIT state");
        if (pseudoDNS.hasIP(mId))
        {
            Serial.printf("DNS arleady has IP");
            if (client.connect(pseudoDNS.getIPById(mId).c_str(), 2883))
            {
                Serial.println("Brick succesfull connected !!");
                stateMachine.switchToState(BrickState::CONNECTED);
            }
            else
            {
                Serial.println("Brick failes connected !!");
            }
        }
        else
        {
            Serial.println("DNS doesnt have IP");
        }
    }
}

void KiKoBrick::handleEnterState(uint8_t state)
{
    Serial.printf("Brick enter state %d\n", state);
    if (state == BrickState::INIT)
    {
        pingTimer.onTimeout(this, &KiKoBrick::onPingTimer);
        protocol.setOnFrameCallback(this, &KiKoBrick::handleProtocolFrame);
        pseudoDNS.addEventListener(this);
        if (!pseudoDNS.isQueriesRunning())
        {
            pseudoDNS.startQueriesForAllHosts();
        }

        if (pseudoDNS.hasIP(mId))
        {
            Serial.printf("DNS arleady has IP");
            if (client.connect(pseudoDNS.getIPById(mId).c_str(), 2883))
            {
                Serial.println("Brick succesfull connected !!");
                stateMachine.switchToState(BrickState::CONNECTED);
            }
            else
            {
                Serial.println("Brick failes connected !!");
            }
        }
        else
        {
            Serial.println("DNS doesnt have IP");
        }
    }
    else if (state == BrickState::CONNECTED)
    {
        pingTimer.start(1000);
        protocol.reset();
    }
}

void KiKoBrick::handleChangeState(uint8_t fromState, uint8_t toState)
{
}

void KiKoBrick::handleExitState(uint8_t state)
{
}

void KiKoBrick::handleProtocolFrame(std::deque<uint8_t> &frame)
{
    uint8_t funciotnCode = ProtocolStd::getUint8_t(frame);
    Serial.printf("KiKoBrick recived frame, code=%d\n", funciotnCode);
    if (funciotnCode >= 0x10)
    {
        handleFrame(funciotnCode, frame);
    }
}

void KiKoBrick::setId(std::string id)
{
    mId = id;
}

void KiKoBrick::begin()
{
    bricks.push_back(this);

    stateMachine.onEnterState(this, &KiKoBrick::handleEnterState);
    stateMachine.onChangeState(this, &KiKoBrick::handleChangeState);
    stateMachine.onExitState(this, &KiKoBrick::handleExitState);
    stateMachine.begin(BrickState::INIT);
}

void KiKoBrick::update()
{
    pseudoDNS.update();

    KiKoTimer::update();

    for (KiKoBrick *brick : bricks)
    {
        brick->loop();
    }
}

void KiKoBrick::loop()
{
    while (client.available())
    {
        protocol.addData(client.read());
    }

    if (client.connected())
    {
        // Serial.println("Client connected");
        // Serial.println("Rozłączono z serwerem");
        // client.stop();
    }
    else
    {
        //Serial.println("Client disconnected");
        if (stateMachine.getCuttentState() == BrickState::CONNECTED)
        {
            stateMachine.switchToState(BrickState::DISCONNECTED);
        }
    }
}

void KiKoBrick::onPingTimer()
{
    Serial.println("KiKoBrick::onPingTimer");
    std::vector<uint8_t> frame;
    ProtocolStd::append(frame, uint8_t(0x01));
    sendProtocolFrame(frame);
}

void KiKoBrick::sendProtocolFrame(const std::vector<uint8_t> &frame)
{

  uint16_t size = frame.size();
  std::vector<uint8_t> datagram;
  ProtocolStd::append(datagram, size);
  datagram.insert(datagram.end(), frame.begin(), frame.end());

    if(client.connected()){
        Serial.println("KiKoBrick write ping frame");
        client.write(datagram.data(), datagram.size());
    }
}
