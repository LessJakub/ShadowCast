#include "Tile.h"

bool Tile::IsWall()
{
    return wall;
}

//bool Tile::IsVisable(Row* row)
//{
//    return (x >= row->start_slope && x <= row->end_slope);
//}