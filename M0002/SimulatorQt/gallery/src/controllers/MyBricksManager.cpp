#include "MyBricksManager.hpp"
#include <QtQml>

MyListModel::MyListModel(QObject *parent) : QAbstractListModel(parent) {
   loadFromSettings();
}

void MyListModel::loadFromSettings()
{
    QSettings settings;
    int size = settings.beginReadArray(SETTNGS_KEY);
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QVariantMap item;
        item["brickName"] = settings.value("brickName").toString();
        item["brickType"] = settings.value("brickType").toString();
        addItem(item);
    }
    settings.endArray();
}

void MyListModel::saveToSettings()
{
    QSettings settings;
    settings.beginWriteArray(SETTNGS_KEY);
    for (int i = 0; i < m_items.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("brickName", m_items[i]["brickName"]);
        settings.setValue("brickType", m_items[i]["brickType"]);
    }
    settings.endArray();
}

int MyListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return m_items.size();
}

QVariant MyListModel::data(const QModelIndex &index, int role) const {
    qDebug() << "MyListModel::data()";
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_items.size())
        return QVariant();

    const QVariantMap &item = m_items.at(index.row());

    if (role == BrickNameRole)
        return item.value("brickName");
    else if (role == BrickTypeRole)
        return item.value("brickType");

    return QVariant();
}

QHash<int, QByteArray> MyListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[BrickNameRole] = "brickName";
    roles[BrickTypeRole] = "brickType";
    return roles;
}

void MyListModel::addBrick(const QString &brickType, const QString &brickName, const QString &brickIp)
{
    if (brickExists(brickType, brickName)) {
       emit brickAlreadyAdded(brickType, brickName);
        return;
    }

    QVariantMap item = {{"brickName", brickName}, {"brickType", brickType}};
    addItem(item);
}

bool MyListModel::brickExists(const QString &brickType, const QString &brickName) const
{
    for (const QVariantMap &existingItem : m_items) {
        if (existingItem.value("brickName") == brickName && existingItem.value("brickType") == brickType) {
            return true;
        }
    }
    return false;
}

void MyListModel::addItem(const QVariantMap &item) {
    qDebug() << "MyListModel::addItem()";
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items.append(item);
    saveToSettings();
    endInsertRows();
}

static void registerMyBricksManagerTypes() {
    qmlRegisterType<MyListModel>("Backend", 1, 0, "MyListModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerMyBricksManagerTypes)

