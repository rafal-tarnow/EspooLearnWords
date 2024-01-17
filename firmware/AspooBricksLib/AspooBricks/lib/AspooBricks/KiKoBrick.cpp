#include "KiKoBrick.h"

PseudoDNS KiKoBrick::pseudoDNS;
std::vector<KiKoBrick *> KiKoBrick::bricks;

KiKoBrick::KiKoBrick()
{
    Serial.println("Create new AspooBrick object");
    bricks.push_back(this);
}

void KiKoBrick::handleOnHostFound(const std::string &id, const std::string &type, const std::string &name, const std::string &ip)
{
}

void KiKoBrick::handleEnterState(uint8_t state)
{
    Serial.printf("Brick enter state %d\n", state);
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
    // Serial.printf("KiKoBrick recived frame, code=%d\n", funciotnCode);
    if (funciotnCode == 0x01)
    {
        timeoutTimer.reset();
    }
    else if (funciotnCode >= 0x10)
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

    desiredState = BrickState::CONNECTED;
}

void KiKoBrick::disconnect()
{
    client.stop();
    desiredState = BrickState::DISCONNECTED;
    currentState = BrickState::DISCONNECTED;

    pingTimer.stop();
    timeoutTimer.stop();
    if (callbackConnectionFunction)
    {
        callbackConnectionFunction(false);
    }
    if (callbackConnectionMethod)
    {
        callbackConnectionMethod(false);
    }
}

bool KiKoBrick::isConnected()
{
    return currentState == BrickState::CONNECTED;
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

    // Serial.printf("Current state %d\n", currentState);
    // Serial.printf("Desired state %d\n", desiredState);

    if (currentState == BrickState::UNINITED)
    {
        pingTimer.onTimeout(this, &KiKoBrick::handlePingTimer);
        timeoutTimer.onTimeout(this, &KiKoBrick::handleTimeoutTimer);
        protocol.setOnFrameCallback(this, &KiKoBrick::handleProtocolFrame);
        pseudoDNS.addEventListener(this);
        protocol.reset();
        currentState = BrickState::INITED;
    }
    else
    {
        if (desiredState == BrickState::CONNECTED && currentState != BrickState::CONNECTED)
        {
            if (!pseudoDNS.isQueriesRunning())
            {
                pseudoDNS.startQueriesForAllHosts();
            }

            if (pseudoDNS.hasIP(mId))
            {
                Serial.printf("client.connect()\n");
                client.connect(pseudoDNS.getIPById(mId).c_str(), 2883);
            }
        }

        if (desiredState == BrickState::CONNECTED && currentState != BrickState::CONNECTED)
        {
            if (client.connected())
            {
                currentState = BrickState::CONNECTED;
                protocol.reset();
                pingTimer.start(1000);
                timeoutTimer.start(5000);

                if (callbackConnectionFunction)
                {
                    callbackConnectionFunction(true);
                }
                if (callbackConnectionMethod)
                {
                    callbackConnectionMethod(true);
                }
            }
        }

        if (desiredState == BrickState::CONNECTED && currentState == BrickState::CONNECTED)
        {
            if (!client.connected())
            {
                pingTimer.stop();
                timeoutTimer.stop();
                if (callbackConnectionFunction)
                {
                    callbackConnectionFunction(false);
                }
                if (callbackConnectionMethod)
                {
                    callbackConnectionMethod(false);
                }
            }
        }
    }

    while (client.available())
    {
        protocol.addData(client.read());
    }
}

void KiKoBrick::handlePingTimer()
{
    // Serial.println("KiKoBrick::onPingTimer");
    std::vector<uint8_t> frame;
    ProtocolStd::append(frame, uint8_t(0x01));
    sendProtocolFrame(frame);
}

void KiKoBrick::handleTimeoutTimer()
{
    client.stop();
    currentState = BrickState::DISCONNECTED;

    pingTimer.stop();
    timeoutTimer.stop();
    if (callbackConnectionFunction)
    {
        callbackConnectionFunction(false);
    }
    if (callbackConnectionMethod)
    {
        callbackConnectionMethod(false);
    }
}

void KiKoBrick::sendProtocolFrame(const std::vector<uint8_t> &frame)
{

    uint16_t size = frame.size();
    std::vector<uint8_t> datagram;
    ProtocolStd::append(datagram, size);
    datagram.insert(datagram.end(), frame.begin(), frame.end());

    if (client.connected())
    {
        // Serial.println("KiKoBrick write ping frame");
        client.write(datagram.data(), datagram.size());
    }
}
