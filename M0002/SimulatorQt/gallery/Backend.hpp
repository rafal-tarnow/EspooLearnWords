#pragma once

#include <map>

#include <QObject>
#include <QString>
#include <qqml.h>
#include "pseudo_dns.hpp"
#include "MyBricksManager.hpp"
#include "T0002Controller.hpp"
#include "K0002Controller.hpp"
#include "./src/ObjectCounter.hpp"
#include "AudioInfo.hpp"

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(PseudoDNSServer* dns READ getDNS CONSTANT)
    Q_PROPERTY(MyBricksList* myBricks READ getMyBricks CONSTANT)
    Q_PROPERTY(T0002Controller* t0002Controller CONSTANT)
    Q_PROPERTY(AudioInfo* audioInfo READ getAudioInfo CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    PseudoDNSServer *getDNS(){
        return &pseudoDNS;
    }

    MyBricksList *getMyBricks(){
        return &myBricksList;
        //return nullptr;
    }

    Q_INVOKABLE T0002Controller *getT0002Controller(QString id){
        return t0002Controllers[id.toStdString()];
    }

    Q_INVOKABLE K0002Controller *getK0002Controller(QString id){
        return k0002Controllers[id.toStdString()];
    }

    AudioInfo *getAudioInfo(){
        //return &testAudioInfo;
        return nullptr;
    }

    Q_INVOKABLE AudioInfo *getAudioInfoAt(){
        //return &testAudioInfo;
        return nullptr;
    }

private slots:
    void timerSlot()
    {
        //ObjectCounter::printQt();
    }
    void pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp);
    void bricksList_onBrickAdded(const QString &id, const QString &type, const QString &name);

private:
    void initBricks();
    Controller* initBrick(const QString &brickId, const QString &brickType, const QString &brickName);
    K0002Controller* initK0002Brick(const QString &brickId, const QString &brickName);
    T0002Controller* initT0002Brick(const QString &brickId, const QString &brickName);

private:
    std::map<std::string, T0002Controller *> t0002Controllers;
    std::map<std::string, K0002Controller *> k0002Controllers;

    QString m_userName;
    PseudoDNSServer pseudoDNS;
    MyBricksList myBricksList;
    QTimer testTimer;
    //AudioInfo testAudioInfo;
};
