#include "test_model_2.hpp".h"
#include <QSqlRecord>
#include <QSqlField>
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


SqlQueryModel::SqlQueryModel(QObject *parent) :

   QSqlQueryModel(parent)
{
    auto err = initDb(db);
    if (err.type() != QSqlError::NoError) {
        qCritical() << err.text();
        //return 1;
    }


    this->setQuery("SELECT fullName, address, city FROM words", db);
}

void SqlQueryModel::setQuery(const QString &query, const QSqlDatabase &db) {

   QSqlQueryModel::setQuery(query, db);
   generateRoleNames();
}

void SqlQueryModel::setQuery(const QSqlQuery & query) {

   //QSqlQueryModel::setQuery(query);
   //generateRoleNames();
}

void SqlQueryModel::generateRoleNames() {

   m_roleNames.clear();
   for( int i = 0; i < record().count(); i ++) {
       m_roleNames.insert(Qt::UserRole + i, record().fieldName(i).toUtf8());
   }
}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const {

   QVariant value;
   if(role < Qt::UserRole) {
       value = QSqlQueryModel::data(index, role);
   }
   else {
       int columnIdx = role - Qt::UserRole;
       QModelIndex modelIndex = this->index(index.row(), columnIdx);
       value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
   }
   return value;
}

void SqlQueryModel::append(const QString &fullName, const QString &address, const QString &city)
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

void registerSqlQueryModel() {
    qmlRegisterType<SqlQueryModel>("Backend", 1, 0, "SqlQueryModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerSqlQueryModel)
