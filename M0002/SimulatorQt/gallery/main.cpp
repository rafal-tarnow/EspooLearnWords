#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>
#include <QLoggingCategory>
#include <QTimer>
#include <QObject>
#include <QtGlobal>
#include "./src/ObjectCounter.hpp"
#include "Application.hpp"

#if defined(Q_OS_ANDROID)
// install custom message handlet to see logs in adb logcat in android
#include <android/log.h>
const char*const applicationName="KiKo bricks";
void myMessageHandler(
    QtMsgType type,
    const QMessageLogContext& context,
    const QString& msg
    ) {
    QString report= "KIKO " + msg;
    if (context.file && !QString(context.file).isEmpty()) {
        report+=" in file ";
        report+=QString(context.file);
        report+=" line ";
        report+=QString::number(context.line);
    }
    if (context.function && !QString(context.function).isEmpty()) {
        report+=+" function ";
        report+=QString(context.function);
    }
    const char*const local=report.toLocal8Bit().constData();
    switch (type) {
    case QtDebugMsg:
        __android_log_write(ANDROID_LOG_DEBUG,applicationName,local);
        break;
    case QtInfoMsg:
        __android_log_write(ANDROID_LOG_INFO,applicationName,local);
        break;
    case QtWarningMsg:
        __android_log_write(ANDROID_LOG_WARN,applicationName,local);
        break;
    case QtCriticalMsg:
        __android_log_write(ANDROID_LOG_ERROR,applicationName,local);
        break;
    case QtFatalMsg:
    default:
        __android_log_write(ANDROID_LOG_FATAL,applicationName,local);
        abort();
    }
}
#endif



class StateListener : public QObject
{
    Q_OBJECT
public slots:
    void onApplicationStateChanged(Qt::ApplicationState state)
    {
        qDebug() << "OOOOOOOOOOOOOOOOOOOOO Application state changeddddd:" << state;
        // Tutaj możesz umieścić dowolny kod reagujący na zmianę stanu aplikacji
    }
};

static QGuiApplication * guiApp = nullptr;

QGuiApplication* getQGuiApplication(){
    return guiApp;
}

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("PseudoDNSClass.debug=false");

#warning "First app run doesent load material style, it can be chacked by changing app and organization name"
    QGuiApplication::setApplicationName("Aspoo Bricks");
    QGuiApplication::setOrganizationName("Aspoo Labs");

#if defined(Q_OS_ANDROID)
    qInstallMessageHandler(myMessageHandler); // install custom message handlet to see logs in adb logcat in android
#endif

    QGuiApplication app(argc, argv);
    guiApp = &app;

    Application application;
    application.setUserName("My name is KiKo");


    // Tworzymy obiekt naszej klasy StateListener
    StateListener stateListener;
    // Łączymy sygnał applicationStateChanged z funkcją onApplicationStateChanged
    QObject::connect(getQGuiApplication(), &QGuiApplication::applicationStateChanged, &stateListener, &StateListener::onApplicationStateChanged);



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

    engine.rootContext()->setContextProperty("application", &application);

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
    engine.load(QUrl("qrc:/kiko_main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;


    //    QTimer timer;
    //    QObject::connect(&timer, &QTimer::timeout, [&]() {
    //        // Tutaj możesz umieścić kod, który zostanie wykonany po upływie czasu
    //        qDebug() << "Timer timeout!";
    //        ObjectCounter::printQt();


    //    });
    //    timer.start(1000);

    return app.exec();
}

#include "main.moc" // porzebne przy QObject::connect(&app, &QGuiApplication::applicationStateChanged, &stateListener, &StateListener::onApplicationStateChanged);
