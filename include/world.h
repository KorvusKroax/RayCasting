#pragma once

#include <point.h>
#include <wall.h>

class World
{
    public:
        Point points[2] =  {
            Point(-50, 80),
            Point( 50, 80),
        };

        Wall walls[1] = {
            Wall(0, 1, 0, 50),
        };

        World() { }
};
