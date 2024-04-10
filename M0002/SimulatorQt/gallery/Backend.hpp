#pragma once

#include <map>

#include <QObject>
#include <QString>
#include <qqml.h>
#include "BrickFinder.hpp"
#include "MyBricksManager.hpp"
#include "T0002Controller.hpp"
#include "K0002Controller.hpp"
#include "K0004Controller.hpp"
#include "K0007Controller.hpp"
#include "BrickCommunicationWrapper.hpp"
#include "./src/ObjectCounter.hpp"

Q_DECLARE_LOGGING_CATEGORY(BackendClass)

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(BrickFinder* brickFinder READ getBrickFinder CONSTANT)
    Q_PROPERTY(MyBricksList* myBricks READ getMyBricks CONSTANT)
    Q_PROPERTY(T0002Controller* t0002Controller CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);
    void applicationActive();
    void applicationSuspended();
    ~Backend();

    BrickFinder *getBrickFinder();
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
    Q_INVOKABLE K0004Controller *getK0004Controller(QString id);
    Q_INVOKABLE K0007Controller *getK0007Controller(QString id);


public slots:
    void handleApplicationStateChanged(Qt::ApplicationState state);

private slots:
    void timerSlot()
    {
        //ObjectCounter::printQt();
    }

    void handleBricksList_onBrickAdded(const QString &id, const QString &type, const QString &name);
    void handleBricksList_onBrickRemoved(const QString &id, const QString &type, const QString &name);

private:
    void initBricks();
    Controller * initBrick(const QString &brickId, const QString &brickType, const QString &brickName);

private:
    std::map<std::string, BrickCommunicationWrapper *> bricksWrappers; //key = id

    QString m_userName;
    BrickFinder mBrickFinder;
    MyBricksList myBricksList;
    QTimer testTimer;
};
