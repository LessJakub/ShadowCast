#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include<memory>
#include <QGraphicsScene>
#include "mapwidget.h"

class battleScene : public QGraphicsScene
{
public:
    battleScene();

    //Pointer to mapWidget. It is used update fov when one of the objects is changed
    MapWidget* mapWidget;

    void changed(const QList<QRectF>& region);
};

#endif // BATTLESCENE_H
