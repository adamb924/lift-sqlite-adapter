#ifndef LIFTQUERYMODEL_H
#define LIFTQUERYMODEL_H

#include <QAbstractItemModel>
#include <QSqlQuery>

#include "liftquerynode.h"
#include "liftdatabase.h"

class LiftQueryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit LiftQueryModel(const LiftDatabase &ldb, QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void setQueryString(const QString &query);

private slots:
    void performModelReset();

private:
    QSqlQuery mQuery;
    QList<LiftQueryNode*> mNodes;
    LiftDatabase mLdb;
};

#endif // LIFTQUERYMODEL_H
