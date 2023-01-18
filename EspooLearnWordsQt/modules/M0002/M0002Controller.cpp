#include "M0002Controller.h"
#include <QtQml>

M0002Controller::M0002Controller(QObject *parent) : QObject(parent) {}

M0002Controller::~M0002Controller()
{
  if (module) {
    delete module;
  }
}

void M0002Controller::initialize(QString name)
{
  deviceName = name;
  module = new M0002(deviceName);
  qDebug() << "M0002Controller::initialize with name = " << deviceName;
  connect(module, &M0002::deviceInited, this, &M0002Controller::deviceInitedEvent);
}

void M0002Controller::deviceInitedEvent(){
    qDebug() << "M0002Controller::deviceInitedEvent Module Inited";
    emit deviceInited();
}

void M0002Controller::setNetworkConfiguration(bool ap_mode) { module->executeApiCommand(M0002::SET_NETWORK_CONFIG, ap_mode); }

void M0002Controller::setDeviceName(const QString &name)
{
  deviceName = name;
  qDebug() << "M0002Controller:: setDeviceName = " << deviceName;
  Q_EMIT deviceNameChanged();
}

QString M0002Controller::getDeviceName() const { return deviceName; }

void M0002Controller::setTurnOn(const bool &state)
{
  qDebug() << "M0002Controller:: turnOnState = " << state;
  if (state) {
    module->executeApiCommand(M0002::TURN_ON_LED);
  }
  else {
    module->executeApiCommand(M0002::TURN_OFF_LED);
  }
}

bool M0002Controller::getAPMode() const {
    qDebug() << "M0002Controller::getAPMode() = " << module->getApMode();
    return module->getApMode();
}

bool M0002Controller::getTurnOn() const {
    qDebug() << "M0002Controller::getTurnOn() = " << module->getOnOff();
    return module->getOnOff();
}

void registerQmlM0002Types() { qmlRegisterType<M0002Controller>("Backend", 1, 0, "M0002Controller"); }

Q_COREAPP_STARTUP_FUNCTION(registerQmlM0002Types)
