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
  connect(module, &M0001::deviceInited, this, &M0001Controller::deviceInitedEvent);
}

void M0001Controller::deviceInitedEvent(){
    qDebug() << "M0001Controller::deviceInitedEvent Module Inited";
    emit deviceInited();
}

void M0001Controller::setNetworkConfiguration(bool ap_mode) { module->executeApiCommand(M0001::SET_NETWORK_CONFIG, ap_mode); }

void M0001Controller::setDeviceName(const QString &name)
{
  deviceName = name;
  qDebug() << "M0001Controller:: setDeviceName = " << deviceName;
  Q_EMIT deviceNameChanged();
}

QString M0001Controller::getDeviceName() const { return deviceName; }

void M0001Controller::setTurnOn(const bool &state)
{
  qDebug() << "M0001Controller:: turnOnState = " << state;
  if (state) {
    module->executeApiCommand(M0001::TURN_ON_LED);
  }
  else {
    module->executeApiCommand(M0001::TURN_OFF_LED);
  }
}



bool M0001Controller::getAPMode() const {
    qDebug() << "M0001Controller::getAPMode() = " << module->getApMode();
    return module->getApMode();
}

bool M0001Controller::getTurnOn() const {
    qDebug() << "M0001Controller::getTurnOn() = " << module->getOnOff();
    return module->getOnOff();
}

void registerQmlM0001Types() { qmlRegisterType<M0001Controller>("Backend", 1, 0, "M0001Controller"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlM0001Types)
