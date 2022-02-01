#pragma once

#include <QAbstractListModel>
#include <QSqlDatabase>

class TestModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ContactRole {
        FullNameRole = Qt::UserRole,
        AddressRole ,
        CityRole,
    };
    Q_ENUM(ContactRole)

    TestModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE QVariantMap get(int row) const;
    Q_INVOKABLE void append(const QString &fullName, const QString &address, const QString  &city);
    Q_INVOKABLE void set(int row, const QString &fullName, const QString &address, const QString  &city);
    Q_INVOKABLE void remove(int row);

private:
    struct Contact {
        QString fullName;
        QString address;
        QString city;
    };

    QSqlDatabase db;
    QString queryTemplate;
};
