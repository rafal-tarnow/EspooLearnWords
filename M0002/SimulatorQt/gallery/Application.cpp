#include "Application.hpp"
#include <QtQml>

Application::Application(QObject *parent) :
    QObject(parent)//, testAudioInfo(this)
{
    pseudoDNS.startQueriesForAllHosts();

    QObject::connect(&testTimer, &QTimer::timeout, this, &Application::timerSlot);

    // Ustawienie interwału czasowego na 1000 ms (1 sekunda)
    testTimer.start(1000);
}

QString Application::userName()
{
    return m_userName;
}

void Application::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

static void registerApplicationTypes()
{
    qRegisterMetaType<CustomType>("CustomType");
    qmlRegisterType<CustomType>("Backend", 1, 0, "CustomType");
}

Q_COREAPP_STARTUP_FUNCTION(registerApplicationTypes)
