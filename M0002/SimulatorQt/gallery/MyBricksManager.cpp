#include "MyBricksManager.hpp"
#include <QtQml>

MyBricksList::MyBricksList(QObject *parent) : QAbstractListModel(parent) {
    loadFromSettings();
}

void MyBricksList::loadFromSettings()
{
    QSettings settings;
    int size = settings.beginReadArray(SETTNGS_KEY);
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        append(settings.value("brickId").toString(), settings.value("brickType").toString(), settings.value("brickName").toString());
    }
    settings.endArray();
}

void MyBricksList::saveToSettings()
{
    QSettings settings;
    settings.beginWriteArray(SETTNGS_KEY);
    for (int i = 0; i < m_items.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("brickId", m_items.at(i).id);
        settings.setValue("brickType", m_items.at(i).type);
        settings.setValue("brickName", m_items.at(i).name);
    }
    settings.endArray();
}

int MyBricksList::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return m_items.count();
}

QVariant MyBricksList::data(const QModelIndex &index, int role) const {

    if (index.row() < rowCount())
        switch (role) {
        case IdRole: return m_items.at(index.row()).id;
        case TypeRole: return m_items.at(index.row()).type;
        case NameRole: return m_items.at(index.row()).name;
        default: return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> MyBricksList::roleNames() const {
    static const QHash<int, QByteArray> roles {
                                              { IdRole, "brickId" },
                                              { TypeRole, "brickType" },
                                              { NameRole, "brickName" },
                                              };
    return roles;
}

bool MyBricksList::append(const QString & id, const QString &brickType, const QString &brickName)
{
    //qDebug() << "MyListModel::addBrick() " << "id=" << id << " type=" << brickType << " name=" << brickName << " ip=" << brickIp;
    if (brickExists(id)) {
        emit brickAlreadyAdded(brickType, brickName);
        return false;
    }

    int row = 0;
    while (row < m_items.count() && brickName > m_items.at(row).name)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, {id, brickType, brickName});
    saveToSettings();
    emit brickAdded(id, brickType, brickName);
    endInsertRows();
    return true;
}

void MyBricksList::set(int row, const QString &id, const QString &brickType, const QString &brickName)
{
    qDebug() << "MyBricksList::set() " << row << " " << id << " " << brickType << " " << brickName;
    if (row < 0 || row >= m_items.count())
        return;

    m_items.replace(row, { id, brickType, brickName });
    saveToSettings();
    emit dataChanged(index(row, 0), index(row, 0), { IdRole, TypeRole, NameRole});
}

void MyBricksList::set(int row, const QString &brickName)
{
    qDebug() << "MyBricksList::set() " << row << " " << brickName;
    if (row < 0 || row >= m_items.count())
        return;

    Brick brick = m_items.at(row);
    brick.name = brickName;
    m_items.replace(row, brick);
    saveToSettings();
    emit dataChanged(index(row, 0), index(row, 0), { IdRole, TypeRole, NameRole});
}

void MyBricksList::remove(int row)
{
    if (row < 0 || row >= m_items.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    Brick brickTmp = m_items.at(row);
    m_items.removeAt(row);
    saveToSettings();
    endRemoveRows();
    emit brickRemoved(brickTmp.id, brickTmp.type, brickTmp.name);
}

void MyBricksList::renameBrick(const QString &id, const QString &name)
{
    qDebug() << "MyBricksList::renameBrick()";
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i).id == id) {
            m_items[i].name = name;
            saveToSettings();
            emit dataChanged(index(i, 0), index(i, 0), {NameRole});
            return;
        }
    }
}

bool MyBricksList::brickExists(const QString &brickId) const
{
    for (const Brick &brick : m_items) {
        if (brick.id == brickId) {
            return true;
        }
    }
    return false;
}

static void registerMyBricksListTypes() {
    qmlRegisterType<MyBricksList>("Backend", 1, 0, "MyBricksList");
}

Q_COREAPP_STARTUP_FUNCTION(registerMyBricksListTypes)
