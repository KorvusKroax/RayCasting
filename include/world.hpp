#pragma once

#include <point.hpp>
#include <wall.hpp>

class World
{
    public:
        Point points[2] =  {
            Point(-63.0f, 25.0f),
            Point(64.0f, 35.0f),
        };

        Wall walls[1] = {
            Wall(0, 1, 128.0f),
        };

        World() { }
};
