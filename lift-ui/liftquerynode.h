#ifndef LIFTQUERYNODE_H
#define LIFTQUERYNODE_H

#include <QList>
#include <QVariant>

class LiftQueryNode
{
public:
    enum Type { Word, Gloss, Definition };

    LiftQueryNode(int selfRow, const QVariant & displayData, LiftQueryNode * parent = nullptr);
    ~LiftQueryNode();

    QVariant displayData() const;

    QList<LiftQueryNode *> *children();

    int selfRow() const;

    LiftQueryNode *parent() const;

    qlonglong entryId() const;

    void setEntryId(const qlonglong &entryId);

    Type type() const;
    void setType(const Type &type);

private:
    qlonglong mEntryId;
    int mSelfRow;
    QVariant mDisplayData;
    QList<LiftQueryNode*> mChildren;
    LiftQueryNode * mParent;
    Type mType;
};

#endif // LIFTQUERYNODE_H
