#include "battlescene.h"
#include<iostream>

battleScene::battleScene()
{

}

void battleScene::changed(const QList<QRectF>& region)
{
	QGraphicsScene::changed(region);
}
