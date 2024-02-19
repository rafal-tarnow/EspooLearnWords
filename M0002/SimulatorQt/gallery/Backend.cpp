#include "Backend.hpp"
#include <QtQml>

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    pseudoDNS.startQueriesForAllHosts();
    connect(&pseudoDNS, &PseudoDNSServer::hostFound, this, &Backend::pseudoDNS_onHostFound);
    connect(&testTimer, &QTimer::timeout, this, &Backend::timerSlot);
    connect(&mT0002Controller, &T0002Controller::brickConnected,
            [this]() {
                this->mT0002Controller.cmdGetInfo();
            });
    connect(&mM0002Controller, &M0002Controller::brickConnected,
            [this]() {
                this->mM0002Controller.cmdGetInfo();
            });

    testTimer.start(1000);
}

QString Backend::userName()
{
    return m_userName;
}

void Backend::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

void Backend::pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp)
{
    qDebug() << "Application::pseudoDNS_onHostFound: " << hostId << " " << hostType << " " << hostName << " " << hostIp;
    if(hostId == "T000215978085"){
        mT0002Controller.connectToBrick(hostIp);
    }else if(hostId == "M000216080620"){
        mM0002Controller.connectToBrick(hostIp);
    }

}
