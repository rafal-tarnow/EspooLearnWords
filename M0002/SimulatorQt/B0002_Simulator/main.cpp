#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

     QIcon::setThemeName("gallery");
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/B0002_Simulator/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
