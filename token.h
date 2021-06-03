#ifndef TOKEN_H
#define TOKEN_H


#include <QGraphicsItem>
#include<QPainter>
#include"entity.h"

class FieldOfView;

class Token : public QGraphicsItem, public Entity
{
    friend QDataStream &operator<<(QDataStream &, const Token &);
    friend QDataStream &operator>>(QDataStream &, Token &);
public:
    Token();
    Token(QColor coulur, QString name);
    virtual ~Token() {};

    //Bounds or limits of the object, determine size of the item
    QRectF boundingRect() const override;

    //paint event function, requires override when inherited (stated in Qt QGraphicsItem documentation)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    //Name of the token, displayed on the tokenControlers list
    QString name;

    //Colour of the token, this value is by default set to blue, can by changed in token creation window.
    QColor colour;

    //Boolean value describing if token is hidden while on not reveald tile.
    bool alwaysVisable;

    //Changes name to newName. Used by token creator.
    void SetName(QString newName);

    //Changes colour to newColur. Used by token creator.
    void SetColour(QColor newColour);

    //Gets and saves reverence to FoV, it is used to update FoV when token moves.
    void getPtrFov(std::shared_ptr<FieldOfView>& ptr);
protected:
    //Method called when item changes. When position of item changes fov will be updated.
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    
    std::shared_ptr<FieldOfView> fov;
};

#endif // TOKEN_H
