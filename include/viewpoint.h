#pragma once

#include <point.h>

class Viewpoint
{
    public:
        Point pos = Point(0, 0);
        float height = 24;
        float heading = 0; // in rad
        float speed = 50;

        Viewpoint() { }
};
