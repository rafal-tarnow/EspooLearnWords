#include "Backend.hpp"
#include <QtQml>

Q_LOGGING_CATEGORY(BackendClass, "BackendClass")

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    qCDebug(BackendClass) << __FUNCTION__;
    initBricks();

    connect(&myBricksList, &MyBricksList::brickAdded, this, &Backend::handleBricksList_onBrickAdded);
    connect(&myBricksList, &MyBricksList::brickRemoved, this, &Backend::handleBricksList_onBrickRemoved);
    connect(&testTimer, &QTimer::timeout, this, &Backend::timerSlot);

    mBrickFinder.startQueriesForAllHosts();
}

void Backend::applicationActive(){
    qCDebug(BackendClass) << __FUNCTION__;
    //testTimer.start(10000);
}

void Backend::applicationSuspended(){
    qCDebug(BackendClass) << __FUNCTION__;
    //testTimer.stop();
}

Backend::~Backend()
{
    qCDebug(BackendClass) << __FUNCTION__;
    for (auto it = bricksWrappers.begin(); it != bricksWrappers.end(); ++it) {
        delete it->second;
    }
    bricksWrappers.clear();
}


BrickFinder *Backend::getBrickFinder()
{
    return &mBrickFinder;
}

MyBricksList *Backend::getMyBricks(){
    return &myBricksList;
}

T0002Controller *Backend::getT0002Controller(QString id){
    qDebug() << "Backend::getT0002Controller id=" << id;
    auto it = bricksWrappers.find(id.toStdString());

    if (it != bricksWrappers.end()) {
        T0002Controller * t0002Controller = dynamic_cast<T0002Controller*>(it->second->get());
        return t0002Controller;
    }
    return nullptr;
}

J0001Controller *Backend::getJ0001Controller(QString id)
{
    qCDebug(BackendClass) << __FUNCTION__ << " id=" << id;
    auto it = bricksWrappers.find(id.toStdString());

    if (it != bricksWrappers.end()) {
        J0001Controller * j0001Controller = dynamic_cast<J0001Controller*>(it->second->get());
        return j0001Controller;
    }
    return nullptr;
}


K0002Controller *Backend::getK0002Controller(QString id){
    qCDebug(BackendClass) << __FUNCTION__ << " id=" << id;
    auto it = bricksWrappers.find(id.toStdString());

    if (it != bricksWrappers.end()) {
        K0002Controller * k0002Controller = dynamic_cast<K0002Controller*>(it->second->get());
        return k0002Controller;
    }
    return nullptr;
}

K0004Controller *Backend::getK0004Controller(QString id)
{
    qCDebug(BackendClass) << __FUNCTION__ << " id=" << id;
    auto it = bricksWrappers.find(id.toStdString());

    if (it != bricksWrappers.end()) {
        K0004Controller * k0004Controller = dynamic_cast<K0004Controller*>(it->second->get());
        return k0004Controller;
    }
    return nullptr;
}

K0007Controller *Backend::getK0007Controller(QString id)
{
    qCDebug(BackendClass) << __FUNCTION__ << " id=" << id;
    auto it = bricksWrappers.find(id.toStdString());

    if (it != bricksWrappers.end()) {
        K0007Controller * k0007Controller = dynamic_cast<K0007Controller*>(it->second->get());
        return k0007Controller;
    }
    return nullptr;
}


void Backend::handleApplicationStateChanged(Qt::ApplicationState state)
{
    qCDebug(BackendClass) << __FUNCTION__ << " state=" << state;
    if(state == Qt::ApplicationActive){
        applicationActive();
    }else if(state == Qt::ApplicationSuspended){
        applicationSuspended();
    }
}

void Backend::handleBricksList_onBrickAdded(const QString &id, const QString &type, const QString &name)
{
    qCDebug(BackendClass) << __FUNCTION__ << id << type << name;
    initBrick(id, type, name);
}


void Backend::handleBricksList_onBrickRemoved(const QString &id, const QString &type, const QString &name)
{
    qCDebug(BackendClass) << __FUNCTION__;
    auto it = bricksWrappers.find(id.toStdString());
    if (it != bricksWrappers.end()) {
        delete it->second;
        bricksWrappers.erase(it);
    }
}


void Backend::initBricks()
{
    qCDebug(BackendClass) << __FUNCTION__;
    for (int i = 0; i < myBricksList.rowCount(); ++i) {
        QString brickId = myBricksList.data(myBricksList.index(i), MyBricksList::IdRole).toString();
        QString brickType = myBricksList.data(myBricksList.index(i), MyBricksList::TypeRole).toString();
        QString brickName = myBricksList.data(myBricksList.index(i), MyBricksList::NameRole).toString();

        Controller *controller = initBrick(brickId, brickType, brickName);
    }    
}


Controller * Backend::initBrick(const QString &brickId, const QString &brickType, const QString &brickName)
{
    qCDebug(BackendClass) << __FUNCTION__ << " id=" << brickId;
    BrickCommunicationWrapper * brick = new BrickCommunicationWrapper(&myBricksList, &mBrickFinder, brickId, brickType, brickName, this);
    bricksWrappers[brickId.toStdString()] = brick;
    return brick->get();
}


