//#include "mainwindow.h"

//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QHostAddress address1("::ffff:192.168.1.104");
    QHostAddress address2("192.168.1.104");

    if (address1.isEqual(address2)) {
        qDebug() << "Address 1 and Address 2 are equal.";
    } else {
        qDebug() << "Address 1 and Address 2 are different.";
    }

    return app.exec();
}
