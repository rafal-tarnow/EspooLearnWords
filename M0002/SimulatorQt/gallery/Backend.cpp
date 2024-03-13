#include "Backend.hpp"
#include <QtQml>


Backend::Backend(QObject *parent) :
    QObject(parent)
{
#warning "initBricks race condidtion with pseudoDNS onHostFound"
    initBricks();

    connect(&myBricksList, &MyBricksList::brickAdded, this, &Backend::bricksList_onBrickAdded);
    connect(&myBricksList, &MyBricksList::brickRemoved, this, &Backend::bricksList_onBrickRemoved);
    connect(&testTimer, &QTimer::timeout, this, &Backend::timerSlot);

    pseudoDNS.startQueriesForAllHosts();
}

void Backend::applicationActive(){
    qDebug() << "Backend::applicationActive()";
    //pseudoDNS.startQueriesForAllHosts();
    //testTimer.start(10000);
}

void Backend::applicationSuspended(){
    qDebug() << "Backend::applicationSuspended()";
    //pseudoDNS.stopQueries();
    //testTimer.stop();
}

Backend::~Backend()
{
    for (auto it = bricksWrappers.begin(); it != bricksWrappers.end(); ++it) {
        delete it->second;
    }
    bricksWrappers.clear();
}


K0002Controller *Backend::getK0002Controller(QString id){
    qDebug() << "Backend::getK0002Controller id=" << id;
    auto it = bricksWrappers.find(id.toStdString());

    if (it != bricksWrappers.end()) {
        K0002Controller * k0002Controller = dynamic_cast<K0002Controller*>(it->second->get());
        return k0002Controller;
    }
    return nullptr;
}


void Backend::onApplicationStateChanged(Qt::ApplicationState state)
{
    qDebug() << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Backend::onApplicationStateChanged() state=" << state;
    //throw std::runtime_error("Application state changed");
    if(state == Qt::ApplicationActive){
        applicationActive();
    }else if(state == Qt::ApplicationSuspended){
        applicationSuspended();
    }
}


void Backend::bricksList_onBrickAdded(const QString &id, const QString &type, const QString &name)
{
    qDebug() << "Backend::bricksList_onBrickAdded " << id << type << name;
    initBrick(id, type, name);
}


void Backend::bricksList_onBrickRemoved(const QString &id, const QString &type, const QString &name)
{
    qDebug() << "Backend::bricksList_onBrickRemoved " << id << type << name;
}


void Backend::controller_onNameChanged(Controller * controller)
{
    QString id = controller->identifier();
    QString name = controller->name();
    myBricksList.renameBrick(id, name);
}


void Backend::initBricks()
{
    for (int i = 0; i < myBricksList.rowCount(); ++i) {
        QString brickId = myBricksList.data(myBricksList.index(i), MyBricksList::IdRole).toString();
        QString brickType = myBricksList.data(myBricksList.index(i), MyBricksList::TypeRole).toString();
        QString brickName = myBricksList.data(myBricksList.index(i), MyBricksList::NameRole).toString();

        Controller * controller = initBrick(brickId, brickType, brickName);
        connect(controller, &Controller::nameChanged, this, [this, controller](){
                this->controller_onNameChanged(controller);
            }, Qt::AutoConnection);
    }    
}


Controller * Backend::initBrick(const QString &brickId, const QString &brickType, const QString &brickName)
{
    BrickCommunicationWrapper * brick = new BrickCommunicationWrapper(&pseudoDNS, brickId, brickType, brickName, this);
    bricksWrappers[brickId.toStdString()] = brick;
    return brick->get();
    qDebug() << "END Backend::initBrick";
}


