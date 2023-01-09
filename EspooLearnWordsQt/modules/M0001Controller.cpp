#include "M0001Controller.h"
#include <QtQml>

M0001Controller::M0001Controller(QObject *parent) : QObject(parent) {}

M0001Controller::~M0001Controller()
{
  if (module) {
    delete module;
  }
}

void M0001Controller::initialize(QString name)
{
  deviceName = name;
  module = new M0001(deviceName);
  qDebug() << "M0001Controller::initialize with name = " << deviceName;
}

void M0001Controller::setDeviceName(const QString &name)
{
  deviceName = name;
  qDebug() << "M0001Controller:: setDeviceName = " << deviceName;
  Q_EMIT deviceNameChanged();
}

QString M0001Controller::getDeviceName() const { return deviceName; }

void M0001Controller::setTurnOn(const bool &state)
{
  //  turnOnState = state;
  //  qDebug() << "M0001Controller:: turnOnState = " << turnOnState;
  //  if (turnOnState) {
  //    module->executeApiCommand(M0001::TURN_ON_LED);
  //  }
  //  else {
  //    module->executeApiCommand(M0001::TURN_OFF_LED);
  //  }

  //  Q_EMIT turnOnChanged();
}

bool M0001Controller::getTurnOn() const { return turnOnState; }

void registerQmlM0001Types() { qmlRegisterType<M0001Controller>("Backend", 1, 0, "M0001Controller"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlM0001Types)
