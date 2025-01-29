#include "BrickCommunicationWrapper.hpp"

#include "main.hpp"
#include "J0001Controller.hpp"
#include "K0002Controller.hpp"
#include "K0004Controller.hpp"
#include "K0007Controller.hpp"
#include "T0002Controller.hpp"

Q_LOGGING_CATEGORY(BrickCommunicationWrapperClass, "BrickCommunicationWrapperClass")

BrickCommunicationWrapper:: BrickCommunicationWrapper(MyBricksList * brickList, BrickFinder * brickFinder, QString id, QString type, QString name, QObject *parent) : QObject(parent){
    qCDebug(BrickCommunicationWrapperClass) << __PRETTY_FUNCTION__ << id;

    mId = id;
    mBrickFinder = brickFinder;
    mBrickList = brickList;

    if(type == "T0002"){
        mController = new T0002Controller(this, id, name);
    }else if(type == "J0001"){
        mController = new J0001Controller(this, id, name);
    }else if(type == "K0002"){
        mController = new K0002Controller(this, id, name);
    }else if(type == "K0004"){
        mController = new K0004Controller(this, id, name);
    }else if(type == "K0007"){
        mController = new K0007Controller(this, id, name);
    }

    if(mController){
        connect(getQGuiApplication(), &QGuiApplication::applicationStateChanged, this, &BrickCommunicationWrapper::onApplicationStateChanged);

        connect(mController, &Controller::nameChanged, this, &BrickCommunicationWrapper::controller_onNameChanged);

        connect(mBrickFinder, &BrickFinder::hostFound, this, &BrickCommunicationWrapper::brickFinder_onHostFound);

        mReconnectTimer = std::make_unique<QTimer>(parent);
        connect(mReconnectTimer.get(), &QTimer::timeout, this, &BrickCommunicationWrapper::tryReconnect);

        activateBrick();

    }
}

BrickCommunicationWrapper::~BrickCommunicationWrapper()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::~BrickCommunicationWrapper() id=" << mId;
    if(mController){
        delete mController;
    }

}

void BrickCommunicationWrapper::activateBrick()
{
    if(mActive == false){
        mActive = true;
        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::initBrick() id=" << mId;
        if(mController != nullptr){
            connect(mController, &Controller::tcpDisconnected, this,
                    [this](){
                        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::  Brick Disconnected";
                        this->tryReconnect();
                        mReconnectTimer->start(RECONNECT_TIME);
                    });

            connect(mController, &Controller::tcpConnected, this,
                    [this](){
                        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper:: Brick Connected id=" << mId;
                        mReconnectTimer->stop();
                    });

            this->tryReconnect();
            mReconnectTimer->start(RECONNECT_TIME);
        }
    }
}

void BrickCommunicationWrapper::suspendBrick()
{
    qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::uninitBrick()";
    mActive = false;
    if(mController != nullptr){
        disconnect(mController, nullptr, nullptr, nullptr);
        mReconnectTimer->stop();
        mController->disconnectFromBrick();
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
    return mController;
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
    if(mBrickFinder->hasBrickIp(mController->identifier())){
        mController->connectToBrick(mBrickFinder->getIpById(mController->identifier()));
    }else{
        qCDebug(BrickCommunicationWrapperClass) << "BrickCommunicationWrapper::  Reconnection fail because of empty dns";
    }
}

void BrickCommunicationWrapper::brickFinder_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp)
{
    qCDebug(BrickCommunicationWrapperClass) << __FUNCTION__ << "id=" << mId << " " << hostId << " " << hostType << " " << hostName << " " << hostIp;
    if(hostId == mController->identifier()){
        if(mController->tcpState() != QAbstractSocket::ConnectedState){
            mController->tcpAbort();
            mController->connectToBrick(hostIp);
        }
    }
}

void BrickCommunicationWrapper::controller_onNameChanged()
{
    qCDebug(BrickCommunicationWrapperClass) << __FUNCTION__;
    if(mController){
        QString id = mController->identifier();
        QString name = mController->name();
        mBrickList->renameBrick(id, name);
    }
}
