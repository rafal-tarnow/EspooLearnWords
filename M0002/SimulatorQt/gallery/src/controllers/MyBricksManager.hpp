#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QVariantMap>

class MyListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit MyListModel(QObject *parent = nullptr);
    enum Roles {
        BrickTypeRole = Qt::UserRole + 1,
        BrickNameRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
     Q_INVOKABLE void addBrick(const QString &brickType, const QString &brickName, const QString &brickIp);
    void addItem(const QVariantMap &item);

 signals:
     void brickAlreadyAdded(const QString &brickType, const QString &brickName);

private:
     const QString SETTNGS_KEY = "MY_BRICKS_LIST";
    QList<QVariantMap> m_items;
    bool brickExists(const QString &brickType, const QString &brickName) const;
    void loadFromSettings();
    void saveToSettings();
};


