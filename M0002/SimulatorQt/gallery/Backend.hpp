#pragma once

#include <QObject>
#include <QString>
#include <qqml.h>
#include "pseudo_dns.hpp"
#include "MyBricksManager.hpp"
#include "T0002Controller.hpp"
#include "M0002Controller.hpp"
#include "./src/ObjectCounter.hpp"
#include "AudioInfo.hpp"

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(PseudoDNSServer* dns READ getDNS CONSTANT)
    Q_PROPERTY(MyBricksList* myBricks READ getMyBricks CONSTANT)
    Q_PROPERTY(T0002Controller* t0002Controller CONSTANT)
    Q_PROPERTY(AudioInfo* audioInfo READ getAudioInfo CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);

    PseudoDNSServer *getDNS(){
        return &pseudoDNS;
    }

    MyBricksList *getMyBricks(){
        return &myBricksList;
        //return nullptr;
    }

    Q_INVOKABLE T0002Controller *getT0002Controller(QString id){
        if(id == "T000215978085"){
            return &mT0002Controller;
        }
        return nullptr;
    }

    Q_INVOKABLE M0002Controller *getM0002Controller(QString id){
        if(id == "M000216080620"){
            return &mM0002Controller;
        }
        return nullptr;
    }

    AudioInfo *getAudioInfo(){
        //return &testAudioInfo;
        return nullptr;
    }

    Q_INVOKABLE AudioInfo *getAudioInfoAt(){
        //return &testAudioInfo;
        return nullptr;
    }


    QString userName();
    void setUserName(const QString &userName);

signals:
    void userNameChanged();

private slots:
    void timerSlot()
    {
        //ObjectCounter::printQt();
    }
    void pseudoDNS_onHostFound(QString hostId, QString hostType, QString hostName, QString hostIp);

private:
    QString m_userName;
    PseudoDNSServer pseudoDNS;
    MyBricksList myBricksList;
    T0002Controller mT0002Controller;
    M0002Controller mM0002Controller;
    QTimer testTimer;
    //AudioInfo testAudioInfo;
};
