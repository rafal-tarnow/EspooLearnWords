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
#include "Backend.hpp"

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

static QGuiApplication * guiApp = nullptr;

QGuiApplication* getQGuiApplication(){
    return guiApp;
}

int main(int argc, char *argv[])
{
    // QLoggingCategory::setFilterRules("PseudoDNSClass.debug=false");
    // QLoggingCategory::setFilterRules("BrickCommunicationWrapperLog=true");
    //QLoggingCategory::setFilterRules("*.debug=false\n");
    QLoggingCategory::setFilterRules("*.debug=false\n"
                                     "*.warning=false\n"
                                     "BrickCommunicationWrapperClass.debug=false\n"
                                     "BackendClass.debug=true\n"
                                     "BrickFinderClass.debug=false\n"
                                     "ControllerClass.debug=false\n"
                                     "K0004ControllerClass.debug=false\n"
                                     "K0007ControllerClass.debug=false\n"
                                     "TcpConncetionClass.debug=false");

#warning "First app run doesent load material style, it can be chacked by changing app and organization name"
    QGuiApplication::setApplicationName("KiKo Bricks");
    QGuiApplication::setOrganizationName("KIKO Labs");

#if defined(Q_OS_ANDROID)
    qInstallMessageHandler(myMessageHandler); // install custom message handlet to see logs in adb logcat in android
#endif

    QGuiApplication app(argc, argv);
    guiApp = &app;

    Backend backend(&app);

    QObject::connect(getQGuiApplication(), &QGuiApplication::applicationStateChanged, &backend, &Backend::handleApplicationStateChanged);

    QIcon::setThemeName("gallery");
    QQuickStyle::setStyle(QLatin1String("Material"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", &backend);
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
