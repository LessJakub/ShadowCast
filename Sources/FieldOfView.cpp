#include "FieldOfView.h"
#include<iostream>
#include<typeinfo>
#include<memory>
#include<thread>
#include<fstream>
#include<math.h>

void FieldOfView::Initializefov(int w, int h)
{
    AllTiles.reserve(h);
    for (int y = 0; y < h; y++)
    {
        std::vector<std::shared_ptr<Tile>> tmpVector;
        tmpVector.reserve(w);
        AllTiles.push_back(tmpVector);
        for (int x = 0; x < w; x++)
        {
            AllTiles[y].push_back(std::make_shared<Tile>(x,y));
        }
    }
    shadows = new QImage(w, h, QImage::Format_ARGB32);
}


FieldOfView::~FieldOfView()
{
    _thredCheck = false;
    delete shadows;
    
    for (int y = 0; y < AllTiles.size(); y++)
    {
        AllTiles[y].clear();
    }
    AllTiles.clear();
    JoinAll();
}


void FieldOfView::SetWall(bool state,std::shared_ptr<Tile> tile)
{
    if(tile)
    {
        tile->wall = state;
        changed = true;
    }
}


// Bresenham's line algorithm
void FieldOfView::SetWallLine(bool state,std::shared_ptr<Tile> tile1, std::shared_ptr<Tile> tile2, int r)
{
    if (!tile1 || !tile2)
        return;
    int x1, x2, y1, y2;
    x1 = tile1->x;
    y1 = tile1 ->y;

    x2 = tile2->x;
    y2 = tile2 ->y;

    bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
    std::swap(x1, x2);
    std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;

    int maxX = (int)x2;

    for(int x=(int)x1; x<=maxX; x++)
    {
        if(steep)
        {
            if(InBoundries(y, x))
            {
                
                SetWall(state,AllTiles[x][y]);
                if(r > 1)
                {
                    if(InBoundries(y , x + r / 2))
                        SetWallLine(state,AllTiles[x][y], AllTiles[x+r/2][y], 1);
                    if (InBoundries(y + r / 2, x))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x][y+r/2], 1);
                    if (InBoundries(y, x - r / 2))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x-r/2][y], 1);
                    if (InBoundries(y - r / 2, x))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x][y-r/2], 1);
                    if (InBoundries(y + r / 2,x + r / 2))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x+r/2][y+r/2], 1);
                    if (InBoundries(y - r / 2, x + r / 2))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x+r/2][y-r/2], 1);
                    if (InBoundries(y + r / 2, x - r / 2))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x-r/2][y+r/2], 1);
                    if (InBoundries(y - r / 2, x - r / 2))
                        SetWallLine(state, AllTiles[x][y], AllTiles[x-r/2][y-r/2], 1);
                }
            }

        }
        else
        {
            if(InBoundries(x,y))
            {
                SetWall(state, AllTiles[y][x]);
                if(r > 1)
                {
                    if(InBoundries(x, y + r/2))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y+r/2][x], 1);
                    if (InBoundries(x, y - r / 2))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y-r/2][x], 1);
                    if (InBoundries(x + r / 2, y ))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y][x+r/2], 1);
                    if (InBoundries(x - r / 2, y))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y][x-r/2], 1);
                    if (InBoundries(x + r / 2, y + r / 2))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y+r/2][x+r/2], 1);
                    if (InBoundries(x - r / 2, y + r / 2))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y+r/2][x-r/2], 1);
                    if (InBoundries(x + r / 2, y - r / 2))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y-r/2][x+r/2], 1);
                    if (InBoundries(x - r / 2, y - r / 2))
                        SetWallLine(state, AllTiles[y][x], AllTiles[y-r/2][x-r/2], 1);
                }
                
            }

        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

void FieldOfView::Scan(Token * origin)
{
    if(origin)
    {
        for(int i = 0; i < 4; i++)
        {
            vectorOfThreads.push_back(std::thread(&FieldOfView::ThreadScanRow, this, origin, i));
        }
        changed = false;
    }
}

void FieldOfView::ThreadScanRow(Token * origin,int direction)
{
    std::unique_ptr<Quadrant> quadrant = std::make_unique<Quadrant>(Quadrant(origin->pos().x(), origin->pos().y(), direction));
    ScanRow(std::move(quadrant), quadrant->row);
}

void FieldOfView::Scan(Group* group)
{
    if(group)
    {
        for(auto itr = group->tokensInGroup.begin(); itr != group->tokensInGroup.end(); itr++)
        {
            Scan(itr->second);
        }
    }
    JoinAll();
}

bool FieldOfView::InBoundries(int x, int y)
{
    if (x < 0 || y < 0)
    {
        return false;
    }
    else if ((x < AllTiles[0].size() && x >= 0) && (y >= 0 && y < AllTiles.size()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FieldOfView::ScanRow(std::shared_ptr<Quadrant> current_quadrant, std::shared_ptr<Row> &row)
{
    if (!current_quadrant || !row)
    {
        return;
    }
    std::shared_ptr<Tile> current_tile = nullptr;
    std::shared_ptr<Tile> prev_tile = nullptr;

    if(!_thredCheck) return;

    //Iterate through all tiles in a row
    for (unsigned int a = 0; a < row->vectorOfTiles.size(); a++)
    {
        std::pair<int, int> tmp = current_quadrant->Transform(row->vectorOfTiles[a].first, row->vectorOfTiles[a].second);
        
        if (InBoundries(tmp.first, tmp.second))
        {
            current_tile = AllTiles[tmp.second][tmp.first];
            //current_tile->reveald=false;

                if (current_tile && (current_tile->IsWall() || IsVisable(row->original_start_slope * row->depth + a, row)))
                {
                    current_tile->reveald = true;
                    current_tile->alphaValue = 0;
                    shadows->setPixel(current_tile->x,current_tile->y, qRgba(0, 0, 0, current_tile->alphaValue));
                }

                //Right row
                if (prev_tile && current_tile && prev_tile->IsWall() && !current_tile->IsWall())
                {
                    row->start_slope = row->Slope(row->original_start_slope * row->depth + a);
                }

                //Left row
                if (prev_tile && current_tile && !prev_tile->IsWall() && current_tile->IsWall())
                {
                    
                    std::shared_ptr<Row> next_row = std::make_shared<Row>(Row(row->depth + 1, row->start_slope, row->end_slope));
                    next_row->end_slope = row->Slope(row->original_start_slope * row->depth + a);
                    ScanRow(current_quadrant, next_row);

                }
                //Check if row ends
                prev_tile = current_tile;
            
        }
    }
    if (prev_tile && !prev_tile->IsWall())
    {
        
        std::shared_ptr<Row> next_row = std::make_shared<Row>(row->depth + 1, row->start_slope, row->end_slope);
        ScanRow(current_quadrant, next_row);
    }
}



void FieldOfView::HideTiles()
{
    for (int y = 0; y < AllTiles.size(); y++)
    {
        for (int x = 0; x < AllTiles[0].size(); x++)
        {
            AllTiles[y][x]->reveald = false;
            AllTiles[y][x]->alphaValue = 125;
            shadows->setPixel(x, y, qRgba(0, 0, 0, 125));
        }
    }
}



void FieldOfView::HideWalls()
{
    for (int y = 0; y < AllTiles.size(); y++)
    {
        for (int x = 0; x < AllTiles[0].size(); x++)
        {
            AllTiles[y][x]->wall = false;
        }
    }
}

void FieldOfView::RevealTiles()
{
    for (int y = 0; y < AllTiles.size(); y++)
    {
        for (int x = 0; x < AllTiles[0].size(); x++)
        {
            AllTiles[y][x]->reveald = true;
        }
    }
}

//Delcalre in another class
bool FieldOfView::IsVisable(int x, std::shared_ptr<Row> row)
{
    return ((float)(row->start_slope *row->depth) <= x && x <= (float)(row->end_slope * row->depth) );
}

void FieldOfView::JoinAll()
{
    for(int i = 0; i < vectorOfThreads.size(); i++)
    {
        vectorOfThreads[i].join();
    }
    vectorOfThreads.clear();
}

void FieldOfView::fovChanged()
{
    changed = true;
}
