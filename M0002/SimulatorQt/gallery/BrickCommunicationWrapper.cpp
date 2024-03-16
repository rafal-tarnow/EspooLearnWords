#include "BrickCommunicationWrapper.hpp"
#include "main.hpp"

Q_LOGGING_CATEGORY(BrickCommunicationWrapperClass, "BrickCommunicationWrapperClass")

BrickCommunicationWrapper::BrickCommunicationWrapper(PseudoDNSServer * dnsServer, QString id, QString type, QString name, QObject *parent) : QObject(parent){
    qCDebug(BrickCommunicationWrapperClass) << __PRETTY_FUNCTION__ << id;

    mId = id;
    dns = dnsServer;
    connect(dns, &PseudoDNSServer::hostFound, this, &BrickCommunicationWrapper::pseudoDNS_onHostFound);

    connect(getQGuiApplication(), &QGuiApplication::applicationStateChanged, this, &BrickCommunicationWrapper::onApplicationStateChanged);

    if(type == "T0002"){
        controller = new T0002Controller(this, id, name);
    }else if(type == "K0002"){
        controller = new K0002Controller(this, id, name);
    }

    reconnectTimer = std::make_unique<QTimer>(parent);
    connect(reconnectTimer.get(), &QTimer::timeout, this, &BrickCommunicationWrapper::tryReconnect);

    activateBrick();
}

BrickCommunicationWrapper::~BrickCommunicationWrapper()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::~BrickCommunicationWrapper() id=" << mId;
    if(controller){
        delete controller;
    }

}

void BrickCommunicationWrapper::activateBrick()
{
    if(mActive == false){
        mActive = true;
        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::initBrick() id=" << mId;
        if(controller != nullptr){
            connect(controller, &Controller::brickDisconnected, this,
                    [this](){
                        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::  Brick Disconnected";
                        this->tryReconnect();
                        reconnectTimer->start(RECONNECT_TIME);
                    });

            connect(controller, &Controller::brickConnected, this,
                    [this](){
                        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper:: Brick Connected id=" << mId;
                        reconnectTimer->stop();
                    });

            this->tryReconnect();
            reconnectTimer->start(RECONNECT_TIME);
        }
    }
}

void BrickCommunicationWrapper::suspendBrick()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::uninitBrick()";
    mActive = false;
    if(controller != nullptr){
        disconnect(controller, nullptr, nullptr, nullptr);
        reconnectTimer->stop();
        controller->disconnectFromBrick();
    }
}

void BrickCommunicationWrapper::applicationActive()
{
    qCDebug(BrickCommunicationWrapperClass) << __PRETTY_FUNCTION__ << " id=" << mId << " mActive=" << mActive ;
    activateBrick();
}

void BrickCommunicationWrapper::applicationSuspended()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::applicationSuspended()";
    suspendBrick();
}

Controller *BrickCommunicationWrapper::get()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::get()";
    return controller;
}

void BrickCommunicationWrapper::onApplicationStateChanged(Qt::ApplicationState state)
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::onApplicationStateChanged() state=" << state;
    if(state == Qt::ApplicationActive){
        applicationActive();
    }else if(state == Qt::ApplicationInactive){

    }
    else if(state == Qt::ApplicationSuspended){
        applicationSuspended();
    }
}

void BrickCommunicationWrapper::tryReconnect()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::tryReconnect() id=" << mId;
    if(dns->hasBrickIp(controller->identifier())){
        controller->connectToBrick(dns->getIpById(controller->identifier()));
    }else{
        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::  Reconnection fail because of empty dns";
    }
}

void BrickCommunicationWrapper::pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp)
{
    qCDebug(BrickCommunicationWrapperClass) << __FUNCTION__ << "id=" << mId << " " << hostId << " " << hostType << " " << hostName << " " << hostIp;
    if(hostId == controller->identifier()){
        controller->connectToBrick(hostIp);
    }
}
