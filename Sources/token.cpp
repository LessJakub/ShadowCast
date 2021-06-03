#include "token.h"
#include<QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include<iostream>
#include"battlescene.h"

QDataStream &operator<<(QDataStream &out, const Token &token)
{
    out << token.name;
    out <<token.id;
    out << token.colour;
    out << token.alwaysVisable;
    out << (qint32)token.pos().x();
    out << (qint32)token.pos().y();
    return out;
}

QDataStream &operator>>(QDataStream &out, Token &token)
{
    out >> token.name;
    out >> token.id;
    out >> token.colour;
    out >> token.alwaysVisable;
    qint32 x, y;
    out >> x;
    out >> y;
    token.setPos(x,y);
    return out;
}

Token::Token()
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    alwaysVisable = false;
}

Token::Token(QColor colour, QString name) :
     colour(colour), name(name)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    alwaysVisable = false;
}



QRectF Token::boundingRect() const
{
    qreal penWidth = 1;
            return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                          20 + penWidth, 20 + penWidth);
}

void Token::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
    painter->fillRect(boundingRect(),colour);
}

void Token::SetName(QString newName)
{
    name = newName;
}

void Token::SetColour(QColor newColour)
{
    colour = newColour;
}

void Token::getPtrFov(std::shared_ptr<FieldOfView>& ptr)
{
    fov = ptr;
}



QVariant Token::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case QGraphicsItem::ItemPositionChange:
    {
        if (fov)
        {
            fov->fovChanged();
        }
        break;
    }
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);
}


