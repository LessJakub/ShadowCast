#include "item.h"
#include<QPainter>

#include"FieldOfView.h"

QDataStream& operator<<(QDataStream& out, const Item& token)
{
	out << token.name;
	out << token.id;
	out << token.colour;
	out << token.alwaysVisable;
	out << (qint32)token.pos().x();
	out << (qint32)token.pos().y();
	return out;
}

QDataStream& operator>>(QDataStream& out, Item& token)
{
	out >> token.name;
	out >> token.id;
	out >> token.colour;
	out >> token.alwaysVisable;
	qint32 x, y;
	out >> x;
	out >> y;
	token.setPos(x, y);
	return out;
}

Item::Item()
{

}

Item::~Item()
{
	wallsState.clear();
	previousState.clear();
}

void Item::moveWalls()
{
	applyPreviousState();
	
	saveCurrentState();
}

void Item::applyPreviousState()
{
	if (!previousState.empty() && fov)
	{
		for (int x = previousState[0].first; x <= previousState[1].first; x++)
		{
			for (int y = previousState[0].second; y <= previousState[3].second; y++)
			{
				if (fov->InBoundries(x, y))
				{
					fov->SetWall(false, fov->AllTiles[y][x]);
				}
			}
		}
	}
	previousState.clear();
	for (int i = 0; i < wallsState.size(); i++)
	{
		if (fov->InBoundries(wallsState[i].first, wallsState[i].second))
		{
			fov->SetWall(true, fov->AllTiles[wallsState[i].second][wallsState[i].first]);
		}
	}
	wallsState.clear();
}

void Item::saveCurrentState()
{
	for (int x = pos().x() + boundingRect().topLeft().x(); x <= pos().x() + boundingRect().topRight().x(); x++)
	{
		for (int y = pos().y() + boundingRect().topLeft().y(); y <= pos().y() + boundingRect().bottomLeft().y(); y++)
		{
			if (fov && fov->InBoundries(x, y))
			{
				if (fov->AllTiles[y][x]->IsWall())
				{
					wallsState.push_back(std::pair<int, int>(x, y));
				}
				fov->SetWall(true, fov->AllTiles[y][x]);
			}
		}
	}
	previousState = returnCorners();
}

void Item::checkIfVisible()
{
	if (alwaysVisable)
	{
		setVisible(true);
		return;
	}

	int x = pos().x();
	int y = pos().y();
	
	QPointF topLeft = boundingRect().topLeft();
	QPointF bottomRight = boundingRect().bottomRight();
	 
	for (int tmpX = x + topLeft.x(); tmpX < x + bottomRight.x(); tmpX++)
	{
		if (fov->InBoundries(tmpX, y + topLeft.y() - 1))
		{
			if (fov->AllTiles[y + topLeft.y() - 1][tmpX]->reveald)
			{
				setVisible(true);
				return;
			}
		}
	}
	for (int tmpY = y + topLeft.y() - 1; tmpY < y + bottomRight.y() + 1; tmpY++)
	{
		if (fov->InBoundries(x + topLeft.x() - 1, tmpY))
		{
			if (fov->AllTiles[tmpY][x + topLeft.x() - 1]->reveald)
			{
				setVisible(true);
				return;
			}
		}
		if (fov->InBoundries(x + bottomRight.x() + 1, tmpY))
		{
			if (fov->AllTiles[tmpY][x + bottomRight.x() + 1]->reveald)
			{
				setVisible(true);
				return;
			}
		}
	}
	for (int tmpX = x + topLeft.x(); tmpX < x + bottomRight.x(); tmpX++)
	{
		if (fov->InBoundries(tmpX, y + bottomRight.y() - 1))
		{
			if (fov->AllTiles[y + bottomRight.y() - 1][tmpX]->reveald)
			{
				setVisible(true);
				return;
			}
		}
	}
	setVisible(false);
}

std::vector<std::pair<int, int>> Item::returnCorners()
{
	std::vector<std::pair<int, int>> tmp;
	tmp.push_back(std::pair<int,int>(pos().x() + boundingRect().topLeft().x(), pos().y() + boundingRect().topLeft().y()));
	tmp.push_back(std::pair<int, int>(pos().x() + boundingRect().topRight().x(), pos().y() + boundingRect().topRight().y()));
	tmp.push_back(std::pair<int, int>(pos().x() + boundingRect().bottomRight().x(), pos().y() + boundingRect().bottomRight().y()));
	tmp.push_back(std::pair<int, int>(pos().x() + boundingRect().bottomLeft().x(), pos().y() + boundingRect().bottomLeft().y()));
	
	return tmp;
}



QVariant Item::itemChange(GraphicsItemChange change, const QVariant& value)
{
	switch (change)
	{
	case QGraphicsItem::ItemPositionChange:
	{
		moveWalls();
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
