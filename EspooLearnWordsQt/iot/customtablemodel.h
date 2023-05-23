#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QMultiHash>
#include <QtCore/QRect>
#include <QTimer>

class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomTableModel(QObject *parent = 0);
    ~CustomTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addMapping(QString color, QRect area);
    void clearMapping() { m_mapping.clear(); }


private slots:
    void onTimer();
private:
    QTimer *timer;
    QList<QList<qreal> *> m_data;
    QMultiHash<QString, QRect> m_mapping;
    int m_columnCount = 2;
    int m_rowCount;
    void append(const qreal &x, const qreal &y);
};

#endif // CUSTOMTABLEMODEL_H
