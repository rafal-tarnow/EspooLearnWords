#include "customtablemodel.h"
#include <QtCore/QList>
#include <QtCore/QRandomGenerator>
#include <QtCore/QRect>
#include <QtGui/QColor>

CustomTableModel::CustomTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

    qDebug() << "CustomTableModel()";
    m_rowCount = 15;

    // m_data
    for (int i = 0; i < m_rowCount; i++) {
        QList<qreal> *dataList = new QList<qreal>(m_columnCount);
        for (int k = 0; k < dataList->size(); k++) {
            if(k == 0){
                dataList->replace(k, i);
            }
            else if(k == 1){
                dataList->replace(k, 23.0 + QRandomGenerator::global()->bounded(2));
            }
            else
                dataList->replace(k, QRandomGenerator::global()->bounded(100));
        }
        m_data.append(dataList);
    }
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &CustomTableModel::onTimer);
    timer->start(2000);
}

CustomTableModel::~CustomTableModel()
{
    delete timer;
}

void CustomTableModel::onTimer(){
    qDebug() << "CustomTableModel::onTimer()";
    static qreal time = 15.0;
    time += 1.0f;
    append(time, 25.0f);
}

void CustomTableModel::append(const qreal &x, const qreal &y)
{
//    int row = 0;
//    while (row < mValues.count() && valueName > mValues.at(row).valueName)
//        ++row;
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    QList<qreal> *dataList = new QList<qreal>(m_columnCount);
    dataList->append(x);
    dataList->append(y);
    m_data.append(dataList);
    //mValues.insert(row, {valueName, moduleType, ipAddress, port, serialNumber, row});
    endInsertRows();
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columnCount;
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section % 2 == 0)
            return "x";
        else
            return "y";
    } else {
        return QString("%1").arg(section + 1);
    }
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        for (const QRect &rect : m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CustomTableModel::addMapping(QString color, QRect area)
{
    m_mapping.insert(color, area);
}
