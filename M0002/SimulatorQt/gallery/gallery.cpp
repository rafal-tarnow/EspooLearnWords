#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("PseudoDNSClass.debug=false");

    #warning "First app run doesent load material style, it can be chacked by changing app and organization name"
    QGuiApplication::setApplicationName("Aspoo Bricks");
    QGuiApplication::setOrganizationName("Aspoo Labs");

    QGuiApplication app(argc, argv);

    QIcon::setThemeName("gallery");

    QSettings settings;
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(settings.value("style").toString());

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    const QString styleInSettings = settings.value("style").toString();
    if (styleInSettings.isEmpty())
        settings.setValue(QLatin1String("style"), QQuickStyle::name());

    QQmlApplicationEngine engine;

    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
        QLatin1String("Imagine"), QLatin1String("Material"), QLatin1String("Universal") };
#if defined(Q_OS_MACOS)
    builtInStyles << QLatin1String("macOS");
    builtInStyles << QLatin1String("iOS");
#elif defined(Q_OS_IOS)
    builtInStyles << QLatin1String("iOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QLatin1String("Windows");
#endif

    engine.setInitialProperties({{ "builtInStyles", builtInStyles }});
    engine.load(QUrl("qrc:/aspoo_main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
