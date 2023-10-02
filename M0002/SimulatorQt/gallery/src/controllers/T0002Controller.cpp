#include "T0002Controller.hpp"
#include <QtQml>

static void registerT0002ControllerTypes()
{
    qmlRegisterType<T0002Controller>("Backend", 1, 0, "T0002Controller");
}

Q_COREAPP_STARTUP_FUNCTION(registerT0002ControllerTypes)
