#include "contactmodel.h"
#include "test_model.hpp"
#include "test_model_2.hpp"
#include <QGuiApplication>
#include <QHash>
#include <QQuickStyle>
#include <QQuickView>
#include <QSqlQueryModel>
#include <QString>
#include <QtDebug>
#include <QtQml>
#include <QtSql>
#include <qautostart.h>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  app.setOrganizationName("Espoo");
  app.setOrganizationDomain("espoo.com");
  app.setApplicationName("Espoo words");

  QIcon::setThemeName("espoo");

  Autostart as;
  as.setAutostart(true);

  QQuickStyle::setStyle("Material");

  qmlRegisterType<ContactModel>("Backend", 1, 0, "ContactModel");

  QQmlApplicationEngine engine;
  // engine.rootContext()->setContextProperty("sqlModel", &sqlModel);
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  return app.exec();
}
