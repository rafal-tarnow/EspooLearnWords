#pragma once

#include <map>

#include <QObject>
#include <QString>
#include <qqml.h>
#include "pseudo_dns.hpp"
#include "MyBricksManager.hpp"
#include "T0002Controller.hpp"
#include "K0002Controller.hpp"
#include "BrickCommunicationWrapper.hpp"
#include "./src/ObjectCounter.hpp"

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(PseudoDNSServer* dns READ getDNS CONSTANT)
    Q_PROPERTY(MyBricksList* myBricks READ getMyBricks CONSTANT)
    Q_PROPERTY(T0002Controller* t0002Controller CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);
    void applicationActive();
    void applicationSuspended();
    ~Backend();

    PseudoDNSServer *getDNS(){
        return &pseudoDNS;
    }

    MyBricksList *getMyBricks(){
        return &myBricksList;
        //return nullptr;
    }

    Q_INVOKABLE T0002Controller *getT0002Controller(QString id){
        qDebug() << "Backend::getT0002Controller id=" << id;
        auto it = bricksWrappers.find(id.toStdString());

        if (it != bricksWrappers.end()) {
            T0002Controller * t0002Controller = dynamic_cast<T0002Controller*>(it->second->get());
            return t0002Controller;
        }
        return nullptr;
    }

    Q_INVOKABLE K0002Controller *getK0002Controller(QString id);

public slots:
    void onApplicationStateChanged(Qt::ApplicationState state);

private slots:
    void timerSlot()
    {
        //ObjectCounter::printQt();
    }

    void bricksList_onBrickAdded(const QString &id, const QString &type, const QString &name);
    void bricksList_onBrickRemoved(const QString &id, const QString &type, const QString &name);
    void controller_onNameChanged(Controller * controller);

private:
    void initBricks();
    Controller * initBrick(const QString &brickId, const QString &brickType, const QString &brickName);

private:
    std::map<std::string, BrickCommunicationWrapper *> bricksWrappers;

    QString m_userName;
    PseudoDNSServer pseudoDNS;
    MyBricksList myBricksList;
    QTimer testTimer;
};
