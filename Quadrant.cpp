#include "Quadrant.h"
#include <iostream>

std::pair <int, int> Quadrant::Transform(int x, int y)
{
    if (direction == 0)
    {
        return std::make_pair<int, int>(origin.first + x, origin.second - y);
    }
    if (direction == 1)
    {
        return std::make_pair<int, int>(origin.first + y, origin.second + x);
    }
    if (direction == 2)
    {

        return std::make_pair<int, int>(origin.first + x, origin.second + y);
    }
    if (direction == 3)
    {
        return std::make_pair<int, int>(origin.first - y, origin.second + x);
    }
}


