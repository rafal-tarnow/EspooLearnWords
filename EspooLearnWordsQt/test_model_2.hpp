#pragma once

#include <QSqlQueryModel>

class SqlQueryModel : public QSqlQueryModel {

   Q_OBJECT
public:

   explicit SqlQueryModel(QObject *parent = 0);

    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    void setQuery(const QSqlQuery &query);
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const {	return m_roleNames;	}

    Q_INVOKABLE void append(const QString &fullName, const QString &address, const QString  &city);
 private:
    QSqlDatabase db;
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
 };
