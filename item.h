#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem>
#include <QColor>

#include "token.h"

class FieldOfView;

class Item : public Token
{
    //Used for saving
    friend QDataStream& operator<<(QDataStream&, const Item&);
    //Used for loading
    friend QDataStream& operator>>(QDataStream&,  Item&);
public:
    Item();
    virtual ~Item();
    void moveWalls();

    //Applies previous state of tiles after item is moved. Otherwise items could delete walls while moving
    void applyPreviousState();

    //Saves state of tiles under item before chenging them to walls
    void saveCurrentState();

    //Checks if any point surrounding the item is visable and if it is set item to visible.
    void checkIfVisible();


    //Coordinates of walls under item
    std::vector<std::pair<int,int>> wallsState;

    //Coordinates of corners in previous state
    std::vector<std::pair<int, int>> previousState;

    //Returns coordinates of the corners of item in global position
    std::vector<std::pair<int,int>> returnCorners();

    

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
};

#endif // ITEM_H
