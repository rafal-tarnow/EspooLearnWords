#include "Backend.hpp"
#include <QtQml>

Backend::Backend(QObject *parent) :
    QObject(parent)
{
#warning "initBricks race condidtion with pseudoDNS onHostFound"
    initBricks();

    connect(&myBricksList, &MyBricksList::brickAdded, this, &Backend::bricksList_onBrickAdded);
    connect(&pseudoDNS, &PseudoDNSServer::hostFound, this, &Backend::pseudoDNS_onHostFound);
    connect(&testTimer, &QTimer::timeout, this, &Backend::timerSlot);

    testTimer.start(1000);
    pseudoDNS.startQueriesForAllHosts();
}

Backend::~Backend()
{
    for (auto it = k0002Controllers.begin(); it != k0002Controllers.end(); ++it) {
        delete it->second;
    }
    k0002Controllers.clear();

    for (auto it = t0002Controllers.begin(); it != t0002Controllers.end(); ++it) {
        delete it->second;
    }
    t0002Controllers.clear();
}

void Backend::pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp)
{
    qDebug() << "Application::pseudoDNS_onHostFound: " << hostId << " " << hostType << " " << hostName << " " << hostIp;    
    if(hostType == "K0002"){
        auto it = k0002Controllers.find(hostId.toStdString());
        if (it != k0002Controllers.end()) {
            K0002Controller *ptr = it->second;
            ptr->connectToBrick(hostIp);
        }
    }else if(hostType == "T0002"){
        auto it = t0002Controllers.find(hostId.toStdString());
        if(it != t0002Controllers.end()){
            T0002Controller *ptr = it->second;
            ptr->connectToBrick(hostIp);
        }
    }
}

void Backend::bricksList_onBrickAdded(const QString &id, const QString &type, const QString &name)
{
    initBrick(id, type, name);
}

void Backend::initBricks(){
    for (int i = 0; i < myBricksList.rowCount(); ++i) {
        QString brickId = myBricksList.data(myBricksList.index(i), MyBricksList::IdRole).toString();
        QString brickType = myBricksList.data(myBricksList.index(i), MyBricksList::TypeRole).toString();
        QString brickName = myBricksList.data(myBricksList.index(i), MyBricksList::NameRole).toString();

        initBrick(brickId, brickType, brickName);
    }    
}

Controller* Backend::initBrick(const QString &brickId, const QString &brickType, const QString &brickName){
    Controller *brick;
    if(brickType == "K0002"){
        brick = initK0002Brick(brickId, brickName);
    }else if(brickType == "T0002"){
        brick = initT0002Brick(brickId, brickName);
    }
    if(pseudoDNS.hasBrickIp(brick->identifier())){
        brick->connectToBrick(pseudoDNS.getIpById(brick->identifier()));
    }
    return brick;
}

K0002Controller* Backend::initK0002Brick(const QString &brickId, const QString &brickName){
    K0002Controller *brick = new K0002Controller(this, brickId, brickName);
    k0002Controllers[brickId.toStdString()] = brick;
    connect(brick, &K0002Controller::brickConnected,
            [this, brick]() {
                brick->cmdGetInfo();
            });
    return brick;
}

T0002Controller* Backend::initT0002Brick(const QString &brickId, const QString &brickName){
    T0002Controller *brick = new T0002Controller(this, brickId, brickName);
    t0002Controllers[brickId.toStdString()] = brick;
    connect(brick, &T0002Controller::brickConnected,
            [this, brick]() {
                brick->cmdGetInfo();
            });
    return brick;
}
