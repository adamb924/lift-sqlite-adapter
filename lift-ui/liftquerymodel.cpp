#include "liftquerymodel.h"

#include <QtDebug>
#include <QSqlError>
#include <QColor>

LiftQueryModel::LiftQueryModel(const LiftDatabase &ldb, QObject *parent)
    : QAbstractItemModel(parent),
      mLdb(ldb)
{
    connect( this, SIGNAL(modelReset()), this, SLOT(performModelReset()));
}

void LiftQueryModel::setQueryString(const QString &query)
{
    beginResetModel();
    mQuery = QSqlQuery( mLdb.database() );
    if( !mQuery.prepare("SELECT entry_id,WritingSystem,Form FROM citation WHERE Form REGEXP ?;") )
    {
        qWarning() << "Statement error: " << mQuery.lastError().text() <<mQuery.lastQuery();
        return;
    }
    mQuery.bindValue(0, query);
    if( ! mQuery.exec() )
    {
        qWarning() << "Query error: " << mQuery.lastError().text() << mQuery.lastQuery();
        return;
    }
    endResetModel();
}

QModelIndex LiftQueryModel::index(int row, int column, const QModelIndex &parent) const
{
    if( mNodes.count() == 0 )
        return QModelIndex();

//    qDebug() << "LiftQueryModel::index" << row << column << parent << mNodes.count();
    if( parent.isValid() ) /// child item
    {
        Q_ASSERT(parent.row() < mNodes.count() );
        Q_ASSERT(row < mNodes.at(parent.row())->children()->count());
        return createIndex(row,column, mNodes.at(parent.row())->children()->at( row ) );
    }
    else /// top-level
    {
        Q_ASSERT(row < mNodes.count() );
        return createIndex(row, column, mNodes.at(row) );
    }
}

QModelIndex LiftQueryModel::parent(const QModelIndex &index) const
{
    if( !index.isValid() )
        return QModelIndex();

//    qDebug() << "LiftQueryModel::parent" << index;

    LiftQueryNode * node = static_cast<LiftQueryNode*>( index.internalPointer() );
    if( node->parent() == nullptr ) /// then it's top-level
    {
        return QModelIndex(); /// and its parent is null
    }
    else /// it's a child element
    {
        /// so the parent is a top-level element
        return createIndex( node->parent()->selfRow(), 0, node->parent() );
    }
}

int LiftQueryModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() ) /// child element
    {
        LiftQueryNode * node = static_cast<LiftQueryNode*>( parent.internalPointer() );
        return node->children()->count();
    }
    else
    {
        return mNodes.count();
    }
}

int LiftQueryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

bool LiftQueryModel::hasChildren(const QModelIndex &parent) const
{
    if( !parent.isValid() )
        return mNodes.count() > 0;

    if( parent.parent().isValid() )
        return false;
    else
        return true;
}

bool LiftQueryModel::canFetchMore(const QModelIndex &parent) const
{
    if( parent.isValid() ) /// a child element
    {
        LiftQueryNode * node = static_cast<LiftQueryNode*>( parent.internalPointer() );
        return node->children()->count() == 0;
    }
    else /// a top-level element
    {
        return false;
    }
}

void LiftQueryModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent)
//    qDebug() << "LiftQueryModel::fetchMore" << parent;

    LiftQueryNode * node = static_cast<LiftQueryNode*>( parent.internalPointer() );

    QSqlQuery senseQuery( mLdb.database() );
    if( !senseQuery.prepare("SELECT _id FROM sense WHERE entry_id=?;") )
    {
        qWarning() << "Statement error: " << senseQuery.lastError().text() << senseQuery.lastQuery();
        return;
    }

    QSqlQuery glossQuery( mLdb.database() );
    if( !glossQuery.prepare("SELECT WritingSystem,Form FROM gloss WHERE sense_id=?;") )
    {
        qWarning() << "Statement error: " << glossQuery.lastError().text() << glossQuery.lastQuery();
        return;
    }

    QSqlQuery definitionQuery( mLdb.database() );
    if( !definitionQuery.prepare("SELECT WritingSystem,Form FROM definition WHERE sense_id=?;") )
    {
        qWarning() << "Statement error: " << definitionQuery.lastError().text() << definitionQuery.lastQuery();
        return;
    }

    senseQuery.bindValue(0,node->entryId());
    if( senseQuery.exec() )
    {
        int rowCount = 0;
        while( senseQuery.next() )
        {
            qlonglong senseId = senseQuery.value(0).toLongLong();

            glossQuery.bindValue(0,senseId);
            if( glossQuery.exec() )
            {
                while( glossQuery.next() )
                {
                    LiftQueryNode * newNode = new LiftQueryNode(rowCount, glossQuery.value(1), node );
                    newNode->setType( LiftQueryNode::Gloss );
                    node->children()->append( newNode );
                    rowCount++;
                }
            }
            else
            {
                qWarning() << "Query error: " << glossQuery.lastError().text() << glossQuery.lastQuery();
            }

            definitionQuery.bindValue(0,senseId);
            if( definitionQuery.exec() )
            {
                while( definitionQuery.next() )
                {
                    LiftQueryNode * newNode = new LiftQueryNode(rowCount, definitionQuery.value(1), node );
                    newNode->setType( LiftQueryNode::Definition );
                    node->children()->append( newNode );
                    rowCount++;
                }
            }
            else
            {
                qWarning() << "Query error: " << definitionQuery.lastError().text() << definitionQuery.lastQuery();
            }
        }
    }
    else
    {
        qWarning() << "Query error: " << senseQuery.lastError().text() << senseQuery.lastQuery();
    }
}

QVariant LiftQueryModel::data(const QModelIndex &index, int role) const
{
    QSqlQuery query(mQuery);

    if (!index.isValid())
    {
        return QVariant();
    }

    LiftQueryNode * node = static_cast<LiftQueryNode*>( index.internalPointer() );
    if( role == Qt::DisplayRole || role == Qt::ToolTipRole )
    {
        return node->displayData();
    }
    else if( role == Qt::ForegroundRole )
    {
        switch( node->type() )
        {
        case LiftQueryNode::Word:
            return QVariant();
        case LiftQueryNode::Gloss:
            return QColor( Qt::darkBlue );
        case LiftQueryNode::Definition:
            return QColor( Qt::darkGray );
        }
    }
//    else if( role == Qt::DecorationRole )
//    {
//        switch( node->type() )
//        {
//        case LiftQueryNode::Word:
//            return QVariant();
//        case LiftQueryNode::Gloss:
//            return QColor( Qt::darkBlue );
//        case LiftQueryNode::Definition:
//            return QColor( Qt::darkGray );
//        }
//    }

    return QVariant();
}

Qt::ItemFlags LiftQueryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

void LiftQueryModel::performModelReset()
{
    qDeleteAll(mNodes);
    mNodes.clear();

    int row = 0;
    while( mQuery.next() )
    {
        LiftQueryNode * newNode = new LiftQueryNode( row, mQuery.value(2) );
        newNode->setEntryId(mQuery.value(0).toLongLong());
        mNodes << newNode;
        row++;
    }
}
