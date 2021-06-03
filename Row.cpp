#include "Row.h"
#include <iostream>

Row::~Row()
{
    if (next_row)
    {
        delete next_row;
    }
    vectorOfTiles.clear();
}

Row* Row::Next()
{
    
    Row* tmp = new Row(depth + 1, start_slope, end_slope);
    return tmp;
}

std::vector<std::pair<int, int>> Row::Tiles()
{
    std::vector<std::pair<int, int>> vectorToReturn;
    for (int i = (start_slope * depth); i <= (int)(end_slope*depth + 1); i++)
    {
        std::pair<int, int> tmp = std::make_pair(i, depth);
        vectorToReturn.push_back(tmp);
    }
    return vectorToReturn;
}

float Row::Slope(int x)
{
    return (2 * x - 1)/ (float)(2 * depth);
}