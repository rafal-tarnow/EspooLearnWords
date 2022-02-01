#include "test_model.hpp"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QtQml>


static QSqlError initDb(QSqlDatabase &db)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    // Open database file. The driver creates a new file if it doesn't exist yet.
    db.setDatabaseName("mylibrary.sqlite");
    if (!db.open())
        return db.lastError();

      QSqlQuery query;

    QStringList tables = db.tables();
    if (!tables.contains("words", Qt::CaseInsensitive)) {
        if (!query.exec(QLatin1String("create table words(fullName varchar, city varchar, address varchar)")))
            return query.lastError();
    }




    if (!query.prepare(QLatin1String("insert into words(fullName, city, address) values(?, ?, ?)")))
        return query.lastError();

    auto addBook = [&] (const QString& title, const QString& author, const QString& address = "pusty") {
        query.addBindValue(title);
        query.addBindValue(author);
        query.addBindValue(address);
        query.exec();
    };

    for(int i = 0; i < 10; i ++){

  //  addBook(QString::number(i) + "Mademoiselle de Maupin", "T. Gautier","aaa");
////    addBook("Der Nachsommer", "A. Stifter", "bbb");
////    addBook("L'Education sentimentale", "G. Flaubert", "ccc");
////    addBook("Voyna i mir", "L. Tolstoy");
////        addBook("Mysterier", "K. Hamsun");
////        addBook("The Sound and the Fury", "W. Faulkner");
////       addBook("Tender is the Night", "F. Scott Fitzgerald");

    }


    return QSqlError();
}

TestModel::TestModel(QObject *parent ) : QAbstractListModel(parent)
{

    auto err = initDb(db);
    if (err.type() != QSqlError::NoError) {
        qCritical() << err.text();
        //return 1;
    }

    queryTemplate = "SELECT rowid, fullName, address, city FROM words";
}

int TestModel::rowCount(const QModelIndex &) const
{
    QSqlQuery q;
    q.prepare(QString("SELECT COUNT (*) FROM (") + queryTemplate + ")");
    q.exec();
    int rows= 0;
    if (q.next()) {
        rows= q.value(0).toInt();
    }
    qDebug() << "rows = " << rows;
    return rows;
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{

    if (index.row() < rowCount()){
        QSqlQuery query;

        if(role == FullNameRole)
        {
            if (!query.prepare(queryTemplate + " LIMIT 1 OFFSET ?")){
                qDebug() << query.lastError();
                return QVariant();
            }
            query.addBindValue(index.row());
            query.exec();
            while (query.next()) {
                 return query.value(1).toString();
             }
            return QVariant();
        }else if(role == AddressRole){
            if (!query.prepare(queryTemplate + " LIMIT 1 OFFSET ?")){
                qDebug() << query.lastError();
                return QVariant();
            }
            query.addBindValue(index.row());
            query.exec();
            while (query.next()) {
                 return query.value(2).toString();
             }
            return QVariant();
        }else if(role == CityRole){
            if (!query.prepare(queryTemplate + " LIMIT 1 OFFSET ?")){
                qDebug() << query.lastError();
                return QVariant();
            }
            query.addBindValue(index.row());
            query.exec();
            while (query.next()) {
                 return query.value(3).toString();
             }
            return QVariant();
        }else{
            return QVariant();
        }

    }
    return QVariant();
}

QHash<int, QByteArray> TestModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { FullNameRole, "fullName" },
        { AddressRole, "address" },
        { CityRole, "city" },
    };
    return roles;
}

QVariantMap TestModel::get(int row) const
{

    QSqlQuery query;

        if (!query.prepare(queryTemplate + " LIMIT 1 OFFSET ?")){
            qDebug() << query.lastError();
            return QVariantMap();
        }
        query.addBindValue(row);
        query.exec();
        while (query.next()) {
             return { {"fullName", query.value(1).toString()}, {"address", query.value(2).toString()}, {"city", query.value(3).toString()}};
         }
        return QVariantMap();


}

void TestModel::append(const QString &fullName, const QString &address, const QString &city)
{



    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    QSqlQuery query;
    query.prepare(QLatin1String("insert into words(fullName, city, address) values(?, ?, ?)"));
    query.addBindValue(fullName);
    query.addBindValue(city);
    query.addBindValue(address);
    query.exec();

    endInsertRows();
}

void TestModel::set(int row, const QString &fullName, const QString &address, const QString &city)
{
    if (row < 0 || row >= rowCount())
        return;

    QSqlQuery query;
    query.prepare("UPDATE words SET fullName='" + fullName +"' , address='" + address + "', city='" + city + "' WHERE rowid = (SELECT rowid FROM("  + queryTemplate + ") LIMIT 1 OFFSET ?)");
    query.addBindValue(row);
    if(!query.exec())
        qDebug() << query.lastError().text();
    dataChanged(index(row, 0), index(row, 0), { FullNameRole, AddressRole, CityRole });
}

void TestModel::remove(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    QSqlQuery query;
    if(!query.prepare("DELETE FROM words WHERE rowid in ( SELECT rowid FROM (" + queryTemplate + ") LIMIT 1 OFFSET ?)")){
        qDebug() << query.lastError();
    }
    query.addBindValue(row);
    if(!query.exec()){
        qDebug() << query.lastError();
    }else{
        qDebug() << "Remove sucessufll";
    }
    endRemoveRows();
}

void registerTestModel() {
    qmlRegisterType<TestModel>("Backend", 1, 0, "TestModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerTestModel)
