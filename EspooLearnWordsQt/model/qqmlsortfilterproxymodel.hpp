#pragma once

#include <QSortFilterProxyModel>

class QQmlSortFilterProxyModel : public QSortFilterProxyModel{
    Q_OBJECT
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged)
public:
    QQmlSortFilterProxyModel(QObject *parent = nullptr);

    QString pattern() const;
    void setPattern(const QString& pattern);
    Q_INVOKABLE void remove(int row);

Q_SIGNALS:
    void patternChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_pattern;
};
