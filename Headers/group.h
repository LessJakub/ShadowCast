#ifndef GROUP_H
#define GROUP_H

#include<string>
#include<vector>

#include "token.h"
#include<map>
#include<QListWidget>
#include <QVariant>
//#include<QColor>

class Group
{
    //<< operator used in saving the file
    friend QDataStream &operator<<(QDataStream &, const Group &);

    //>> operator used in loading the file
    friend QDataStream &operator>>(QDataStream &, Group &);

public:
    Group();
    Group(QString name);
    ~Group();

    //Adds token to group
    bool AddToGroup(Token *tokenToAdd);

    //Sets name of the group
    void SetName(QString newname);

    //Name of the group
    QString name;

    //Map of tokens in group, key value is unique id of token
    std::map<int, Token*> tokensInGroup;

    //Bool value that determines if each element of the group is always visable
    bool alwaysVisable;

    //Number of elements in group
    int groupSize;

    //Unique id of the group
    int groupId;

    //Value incremented with creation of each group, used for unique groups id
    static int groupCounter;

    //Required for Qt comboboxes, stores id of this group
    QVariant * variantOfThis;

};

#endif // GROUP_H
