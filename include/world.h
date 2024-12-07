#pragma once

#include <point.h>
#include <wall.h>

class World
{
    public:
        Point points[4] =  {
            Point(-32, 64),
            Point( 32, 64),
            Point(-100, 50),
            Point( 100, 100),
        };

        Wall walls[3] = {
            Wall(0, 1, 0, 64),
            Wall(2, 0, 0, 64),
            Wall(1, 3, 0, 64),
        };

        World() { }
};
