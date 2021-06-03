#pragma once
//#include "Row.h"

class Tile
{
public:
    Tile() : x(0), y(0) { wall = false; reveald = false; }
    Tile(int X, int Y) : x(X), y(Y) { wall = false; reveald = true; alphaValue = 125; };

    //Boolean value descibing if tile is a wall
    bool IsWall();

    //Coordinate x in global perspective
    int x;
    //Coordinate y in global perspective
    int y;
    //Alpha value of tile, used in creating the shadows image
    int alphaValue;

    //Boolean value describing if tile is revealed by the light
    bool reveald;

    //Boolean value describing if tile is a wall (not transparent)
    bool wall;
};
