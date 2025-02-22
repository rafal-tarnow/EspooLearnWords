#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QVariantMap>
#include <qqml.h>
#include "./src/ObjectCounter.hpp"

class MyBricksList : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

public:
    enum BrickRole {
        IdRole = Qt::UserRole,
        TypeRole,
        NameRole
    };
    Q_ENUM(BrickRole)

    explicit MyBricksList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE bool append(const QString & id, const QString &brickType, const QString &brickName);
    Q_INVOKABLE void set(int row, const QString &id, const QString &brickType, const QString  &brickName);
    Q_INVOKABLE void set(int row, const QString  &brickName);
    Q_INVOKABLE void remove(int row);
    void renameBrick(const QString &id, const QString &name);

signals:
    void brickAlreadyAdded(const QString &brickType, const QString &brickName);
    void brickAdded(const QString &id, const QString &type, const QString &name);
    void brickRemoved(const QString &id, const QString &type, const QString &name);

private:
    struct Brick {
        QString id;
        QString type;
        QString name;
    };

    const QString SETTNGS_KEY = "MY_BRICKS_LIST";
    QList<Brick> m_items;
    bool brickExists(const QString &brickId) const;
    void loadFromSettings();
    void saveToSettings();
    DBG_COUNT("MyBricksList");
};


