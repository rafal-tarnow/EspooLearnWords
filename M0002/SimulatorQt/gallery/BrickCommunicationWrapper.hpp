#pragma once

#include <QObject>
#include "pseudo_dns.hpp"
#include "K0002Controller.hpp"
#include "T0002Controller.hpp"

class BrickCommunicationWrapper: public QObject{
    Q_OBJECT
public:
    explicit BrickCommunicationWrapper(PseudoDNSServer * dns, QString id, QString type, QString name, QObject *parent = nullptr);
    void applicationActive();
    void applicationSuspended();
    Controller * get();

private slots:
    void onApplicationStateChanged(Qt::ApplicationState state);
    void pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp);
private:
    PseudoDNSServer * dns = nullptr;
    Controller * controller = nullptr;
};
