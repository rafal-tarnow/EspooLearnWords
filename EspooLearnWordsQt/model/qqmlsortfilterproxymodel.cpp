#include "qqmlsortfilterproxymodel.hpp"
#include <QtQml>
#include "../test_model.hpp"

QQmlSortFilterProxyModel::QQmlSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortRole(Qt::UserRole);
    sort(0);
}

QString QQmlSortFilterProxyModel::pattern() const
{
    return m_pattern;
}

void QQmlSortFilterProxyModel::setPattern(const QString& pattern)
{
    if (m_pattern == pattern)
        return;

    m_pattern = pattern;
    Q_EMIT patternChanged();
    invalidate();
}

void QQmlSortFilterProxyModel::remove(int row)
{
    qDebug() << "hooj = " << row;
    QModelIndex indexToRemove = this->index(row,0);
    QModelIndex sourceIndexToRemove = mapToSource(indexToRemove);
    dynamic_cast<TestModel*>(sourceModel())->remove(sourceIndexToRemove.row());
}

bool QQmlSortFilterProxyModel::filterAcceptsRow(int sourceRow,
                                              const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    return(sourceModel()->data(index, Qt::UserRole).toString().contains(m_pattern, Qt::CaseInsensitive) || sourceModel()->data(index, Qt::UserRole + 2).toString().contains(m_pattern, Qt::CaseInsensitive));
}

void registerQQmlSortFilterProxyModelTypes() {
    qmlRegisterType<QQmlSortFilterProxyModel>("SortFilterProxyModel", 0, 2, "SortFilterProxyModel");
}

Q_COREAPP_STARTUP_FUNCTION(registerQQmlSortFilterProxyModelTypes)

