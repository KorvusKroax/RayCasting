#pragma once

#include <point.h>

class Viewpoint
{
    public:
        Point pos;
        float height;
        float heading; // in rad
        float speed;

        Viewpoint(float x = 0.0f, float y = 0.0f, float height = 0.0f, float heading = 0.0f, float speed = 1.0f)
        {
            this->pos = Point(x, y);
            this->height = height;
            this->heading = heading;
            this->speed = speed;
        }
};
