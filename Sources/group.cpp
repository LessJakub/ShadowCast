#include "group.h"
#include<iostream>
#include<typeinfo>
#include <QVariant>

Q_DECLARE_METATYPE(Group);

int Group::groupCounter = 0;

QDataStream &operator<<(QDataStream &out, const Group &group)
{
    out << group.name;
    out << group.groupId;
    out << group.alwaysVisable;
    out << group.groupCounter;
    out << (qint32)group.tokensInGroup.size();
    return out;
}

QDataStream &operator>>(QDataStream &out, Group &group)
{
    out >> group.name;
    out >> group.groupId;
    out >> group.alwaysVisable;
    out >> group.groupCounter;
    out >> group.groupSize;
    return out;
}

Group::Group()
{
    name = "New group";
    groupId = groupCounter++;
    variantOfThis = new QVariant(groupId);
    alwaysVisable = false;
}

Group::Group(QString name) :
    name(name)
{
    groupId = groupCounter++;
    variantOfThis = new QVariant(groupId);
    alwaysVisable = false;
}

Group::~Group()
{
    for (std::map<int, Token*>::iterator itr = tokensInGroup.begin();
        itr != tokensInGroup.end(); itr++)
    {
        delete itr->second;
    }
    tokensInGroup.clear();
    delete variantOfThis;
    variantOfThis = nullptr;
}

bool Group::AddToGroup(Token* tokenToAdd)
{
    if(tokenToAdd)
    {
        tokensInGroup.insert(std::pair<int, Token*>(tokenToAdd->id, std::move(tokenToAdd)));
        groupSize = tokensInGroup.size();
        return true;
    }
    else
    {
        return false;
    }
}

void Group::SetName(QString newname)
{
    name = newname;
}

