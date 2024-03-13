#include "BrickCommunicationWrapper.hpp"
#include "main.hpp"

BrickCommunicationWrapper::BrickCommunicationWrapper(PseudoDNSServer * dnsServer, QString id, QString type, QString name, QObject *parent) : QObject(parent){
    dns = dnsServer;
    connect(dns, &PseudoDNSServer::hostFound, this, &BrickCommunicationWrapper::pseudoDNS_onHostFound);

    connect(getQGuiApplication(), &QGuiApplication::applicationStateChanged, this, &BrickCommunicationWrapper::onApplicationStateChanged);

    if(type == "T0002"){
        controller = new T0002Controller(this, id, name);
    }else if(type == "K0002"){
        controller = new K0002Controller(this, id, name);
    }
}

void BrickCommunicationWrapper::applicationActive()
{
    if(controller != nullptr){
        connect(controller, &Controller::brickDisconnected, this,
                [this](){
                    qDebug() << "Brick Disconnected";
                    controller->connectToBrick(dns->getIpById(controller->identifier()));
                });

        if(dns->hasBrickIp(controller->identifier())){
            controller->connectToBrick(dns->getIpById(controller->identifier()));
        }
    }
}

void BrickCommunicationWrapper::applicationSuspended()
{
    if(controller != nullptr){
        disconnect(controller, nullptr, nullptr, nullptr);
        controller->disconnectFromBrick();
    }
}

Controller *BrickCommunicationWrapper::get()
{
    return controller;
}

void BrickCommunicationWrapper::onApplicationStateChanged(Qt::ApplicationState state)
{
    if(state == Qt::ApplicationActive){
        applicationActive();
    }else if(state == Qt::ApplicationSuspended){
        applicationSuspended();
    }
}

void BrickCommunicationWrapper::pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp)
{
    qDebug() << "Application::pseudoDNS_onHostFound(): " << hostId << " " << hostType << " " << hostName << " " << hostIp;
    if(hostId == controller->identifier()){
        controller->connectToBrick(hostIp);
    }
}
