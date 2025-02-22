#pragma once

#include <QObject>
#include <QLoggingCategory>
#include "pseudo_dns.hpp"
#include "K0002Controller.hpp"
#include "T0002Controller.hpp"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(BrickCommunicationWrapperClass)

class BrickCommunicationWrapper: public QObject{
    Q_OBJECT
public:
    explicit BrickCommunicationWrapper(PseudoDNSServer * dns, QString id, QString type, QString name, QObject *parent = nullptr);
    ~BrickCommunicationWrapper();
    void activateBrick();
    void suspendBrick();
    void applicationActive();
    void applicationSuspended();
    Controller * get();

private slots:
    void onApplicationStateChanged(Qt::ApplicationState state);
    void tryReconnect();
    void pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp);
private:
    int RECONNECT_TIME = 2000;
    PseudoDNSServer * dns = nullptr;
    Controller * controller = nullptr;
    std::unique_ptr<QTimer> reconnectTimer;
    bool mActive = false;
    QString mId;
    DBG_COUNT("BrickCommunicationWrapper");
};
