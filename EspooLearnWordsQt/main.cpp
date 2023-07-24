#include "contactmodel.h"
#include <QGuiApplication>
#include <QApplication>
#include <QHash>
#include <QQuickStyle>
#include <QQuickView>
#include <QSqlQueryModel>
#include <QString>
#include <QtDebug>
#include <QtQml>
#include <QtSql>
#include <qautostart.h>
#include <QString>
#include "./iot/plot/qmlplot.h"
#include "./iot/customtablemodel.h"

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);

  qDebug() << "*****************^^^^^^^^^^^^^^^^^^^^^%^^^^^^^^^^^^^^^^^^^^^^$$$$$$$$$$$$$$$$$$$$$$$$$$#########################@@@@@@@@@@@@@@@@@@@@@@@@@@@2";
  app.setOrganizationName("Aspoo");
  app.setOrganizationDomain("aspoo.com");
  app.setApplicationName("Aspoo words");

  QIcon::setThemeName("espoo");

  qDebug() << "RTT main";

  Autostart as;
  as.setAutostart(true);

  QQuickStyle::setStyle("Material");

  qmlRegisterType<CustomTableModel>("CustomTableModel", 1, 0, "CustomTableModelItem");
  qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");
  qmlRegisterType<ContactModel>("Backend", 1, 0, "ContactModel");

  QQmlApplicationEngine engine;
  // engine.rootContext()->setContextProperty("sqlModel", &sqlModel);
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  return app.exec();
}
