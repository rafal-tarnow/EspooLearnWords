#pragma once

#include <QObject>
#include <QString>
#include <qqml.h>
#include "./src/emulators/pseudo_dns.hpp"
#include "./src/controllers/MyBricksManager.hpp"
#include "./src/controllers/T0002Controller.hpp"
#include "./src/ObjectCounter.hpp"
#include "AudioInfo.hpp"


class CustomType : public QObject {
    Q_OBJECT
    // ... Deklaracje składowych klasy CustomType
    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    explicit CustomType(QObject *parent = nullptr):QObject(parent){
        name = "CustomType";
    }

    QString getName(){
        return name;
    }

private:
    QString name;
};

Q_DECLARE_METATYPE(CustomType)

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(PseudoDNSServer* dns READ getDNS CONSTANT)
    Q_PROPERTY(MyBricksList* myBricks READ getMyBricks CONSTANT)
    Q_PROPERTY(T0002Controller* t0002Controller READ getT0002Controller CONSTANT)
    Q_PROPERTY(AudioInfo* audioInfo READ getAudioInfo CONSTANT)
    QML_ELEMENT

public:
    explicit Application(QObject *parent = nullptr);

    PseudoDNSServer *getDNS(){
        return &pseudoDNS;
    }

    MyBricksList *getMyBricks(){
        return &myBricksList;
        //return nullptr;
    }

    T0002Controller *getT0002Controller(){
        //return &mT0002Controller;
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

private:
    QString m_userName;
    PseudoDNSServer pseudoDNS;
    MyBricksList myBricksList;
    //T0002Controller mT0002Controller;
    QTimer testTimer;
    //AudioInfo testAudioInfo;
};
