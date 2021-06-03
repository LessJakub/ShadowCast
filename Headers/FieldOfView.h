#pragma once
#include<vector>
#include<thread>
#include<memory>
#include<QImage>
#include"Tile.h"
#include"Row.h"
#include"Quadrant.h"
#include"token.h"
#include"group.h"

class FieldOfView
{
public:
    FieldOfView()
    {
        _thredCheck = true;
    };
    FieldOfView(int width, int height)
    {
        Initializefov(width, height);
        _thredCheck = true;
    };
    ~FieldOfView();

    //Sets given tile to a wall
    void SetWall(bool state,std::shared_ptr<Tile> tile);

    //Sets tiles in line to a wall. If range is greater than 1 it will call itself with smaller range in 8 directions
    void SetWallLine(bool state,std::shared_ptr<Tile> tile1, std::shared_ptr<Tile> tile2, int r);

    //Changes every tile to a non wall
    void HideWalls();

    //Changes all tiles to revealed
    void RevealTiles();

    //Skans from a token perspective
    void Scan(Token *origin);

    //Calls Scan(Token *origin) for every Token in group
    void Scan(Group* group);

    //Initializes FoV, creating and filling 2d vector AllTiles
    void Initializefov(int w, int h);

    //Recursivly scan each row in a quadrant
    void ScanRow(std::shared_ptr<Quadrant> current_quadrant, std::shared_ptr<Row> &row);

    //Creates quadrant and gives it as argument to ScanRow function
    void ThreadScanRow(Token * origin,int direction);

    //Changes every tile to revealed
    void HideTiles();

    //Checks if x and y is in boundries of AllTiles
    bool InBoundries(int x, int y);

    //Checks if tile is visible in row at x position by cheking rows slopes
    bool IsVisable(int x, std::shared_ptr<Row> row);


    //Generated shadows as image
    QImage* shadows;

    //Joins all created threads
    void JoinAll();

    //Sets changed to true
    void fovChanged();

    //Used to check if fov was modyfied. Scan wont be run if perspective stays the same
    bool changed;

    //Used to stop threads when FieldOfView is deleted
    std::atomic<bool> _thredCheck;

    //Threads of ScanRow
    std::vector<std::thread> vectorOfThreads;

    //2D vector of Tiles. Coordinates used are first y, second x
    std::vector<std::vector<std::shared_ptr<Tile>>> AllTiles;
};
