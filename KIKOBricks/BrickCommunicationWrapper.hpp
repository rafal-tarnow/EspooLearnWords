#pragma once

#include <QObject>
#include <QLoggingCategory>
#include "BrickFinder.hpp"
#include "MyBricksManager.hpp"
#include "Controller.hpp"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(BrickCommunicationWrapperClass)

class BrickCommunicationWrapper: public QObject{
    Q_OBJECT
public:
    explicit BrickCommunicationWrapper(MyBricksList * brickList, BrickFinder * brickFinder,  QString id, QString type, QString name, QObject *parent = nullptr);
    ~BrickCommunicationWrapper();
    void activateBrick();
    void suspendBrick();
    void applicationActive();
    void applicationSuspended();
    Controller * get();

private slots:
    void onApplicationStateChanged(Qt::ApplicationState state);
    void tryReconnect();
    void brickFinder_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp);
    void controller_onNameChanged();
private:
    int RECONNECT_TIME = 2000;
    BrickFinder * mBrickFinder = nullptr;
    MyBricksList * mBrickList = nullptr;
    Controller * mController = nullptr;
    std::unique_ptr<QTimer> mReconnectTimer;
    bool mActive = false;
    QString mId;
    DBG_COUNT("BrickCommunicationWrapper");
};
