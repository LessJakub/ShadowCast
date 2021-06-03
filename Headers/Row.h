#pragma once
#ifndef ROW_H
#define ROW_H
#include "Tile.h"
#include<vector>
#include<memory>


class Row
{
public:
    Row() {}
    Row(int d, double start_s = 0, double end_s = 0) : 
        depth(d), start_slope(start_s), end_slope(end_s)
    {
        next_row = nullptr;
        vectorOfTiles = Tiles();
        original_start_slope = start_slope;
        original_end_slope = end_slope;
    }
    ~Row();

    //Creates next row and returns pointer to it.
    virtual Row* Next();

    //Function generating coordinates of all tiles in the row.
    std::vector<std::pair<int, int>> Tiles();

    //Slop of tile at x position in the row.
    float Slope(int x);

    //Pointer to next row.
    Row* next_row;

    //Vector of coordinates of tiles.
    std::vector<std::pair<int, int>> vectorOfTiles;

    //Distance from origin of quadrant to this row
    int depth;

    //Starting slope of the row, limites vision from the beggining
    float start_slope;

    //Ending slope of the row, limites vision from the end
    float end_slope;

    //Original starting slope
    float original_start_slope;

    //Original ending slope
    float original_end_slope;


};
#endif // !ROW_H
