#pragma once

struct Point
{
    public:
        float x, y;

        Point(float x = 0, float y = 0) {
            this->x = x;
            this->y = y;
        }

        Point rotate(Point origo, float angle)
        {
            float x = this->x - origo.x;
            float y = this->y - origo.y;

            return Point(
                origo.x + (x * cos(angle) - y * sin(angle)),
                origo.y + (y * cos(angle) + x * sin(angle))
            );
        }
};
