#include <QtDebug>
#include <QString>
#include <QHash>
#include <QGuiApplication>
#include <QtQml>
#include <QQuickView>
#include <QtSql>
#include <QSqlQueryModel>
#include <QQuickStyle>
#include <qautostart.h>
#include "contactmodel.h"
#include "test_model_2.hpp"
#include "test_model.hpp"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Espoo");
    app.setOrganizationDomain("espoo.com");
    app.setApplicationName("Espoo words");

    Autostart as;
    as.setAutostart(true);

    QQuickStyle::setStyle("Material");

    qmlRegisterType<ContactModel>("Backend", 1, 0, "ContactModel");



    QQmlApplicationEngine engine;
   // engine.rootContext()->setContextProperty("sqlModel", &sqlModel);
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}

