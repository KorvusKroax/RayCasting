#pragma once

#include <point.hpp>
#include <wall.hpp>

class World
{
    public:
        Point points[2] =  {
            Point(-50, 80),
            Point( 50, 80),
        };

        Wall walls[1] = {
            Wall(0, 1, 50),
        };

        World() { }
};
