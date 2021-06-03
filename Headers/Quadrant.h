#pragma once
#include "Tile.h"
#include "Row.h"
#include<memory>
class Quadrant
{
public:
    Quadrant(int width, int height, int dir) : direction(dir)
    {
        origin.first = width;
        origin.second = height;
        row = std::make_shared<Row>(Row(1, -1, 1));
    }


    //Returns pair of ints, changes given coordinates in quadrant to coordinates in whole structure.
    std::pair <int, int> Transform(int x, int y);

    /*Direction in which quadrant is facing
    north = 0;
    east = 1;
    south = 2;
    west = 3;
    */
    int direction;

    //Origin tile of quadrant.
    std::pair <int, int> origin;

    //Shared ptr to row created in this quadrant.
    std::shared_ptr<Row> row;
};
