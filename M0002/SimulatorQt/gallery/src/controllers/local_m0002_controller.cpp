#include "local_m0002_controller.hpp"
#include <QtQml>


static void registerLocalM0002ControllerTypes() {
    qmlRegisterType<LocalM0002Controller>("Backend", 1, 0, "LocalM0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerLocalM0002ControllerTypes)
