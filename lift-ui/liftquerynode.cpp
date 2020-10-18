#include "liftquerynode.h"

LiftQueryNode::LiftQueryNode(int selfRow, const QVariant &displayData, LiftQueryNode * parent) :
    mSelfRow(selfRow),
    mDisplayData(displayData),
    mParent(parent),
    mType(LiftQueryNode::Word)
{

}

LiftQueryNode::~LiftQueryNode()
{
    qDeleteAll(mChildren);
}

QVariant LiftQueryNode::displayData() const
{
    return mDisplayData;
}

QList<LiftQueryNode *> *LiftQueryNode::children()
{
    return &mChildren;
}

int LiftQueryNode::selfRow() const
{
    return mSelfRow;
}

LiftQueryNode *LiftQueryNode::parent() const
{
    return mParent;
}

qlonglong LiftQueryNode::entryId() const
{
    return mEntryId;
}

void LiftQueryNode::setEntryId(const qlonglong &entryId)
{
    mEntryId = entryId;
}

LiftQueryNode::Type LiftQueryNode::type() const
{
    return mType;
}

void LiftQueryNode::setType(const Type &type)
{
    mType = type;
}
