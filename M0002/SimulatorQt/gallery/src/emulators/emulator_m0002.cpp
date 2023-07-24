#include "emulator_m0002.hpp"
#include <QtQml>

EmulatorM0002::EmulatorM0002(QObject *parent) : QObject(parent)
{
     qInfo() << "Create EmulatorM0002";
    pseudoDNSServer = std::make_unique<PseudoDNSServer>();
}

EmulatorM0002::~EmulatorM0002()
{
     qInfo() << "Delete EmulatorM0002";
}

void EmulatorM0002::start(const QString& serverName){
     mServerName = serverName;
     pseudoDNSServer->startQueriesForHost(mServerName);
}

static void registerEmulatorM0002Types() {
    qmlRegisterType<EmulatorM0002>("Backend", 1, 0, "EmulatorM0002");
}

Q_COREAPP_STARTUP_FUNCTION(registerEmulatorM0002Types)


